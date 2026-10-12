# Plan: migrate onto the PSX harness package

**Status: proposed 2026-10-10. Nothing executed.** The harness
(`../psx-decompilation-harness`) was changing while this was written: its
scaffolder, `templates/project/` and the removal of `lib/` were all in progress.
Re-check every harness citation below against its tree before executing.
Vendor pins and patches are the harness's responsibility and are out of scope
here.

## The goal, and what the harness does today

The goal: delete this repository's Pi machinery, then
`pi install --local` the harness.

Today's harness does not support that model.
- The package declares only an onboarding extension and skill
  (`package.json` `pi`: `psx-bootstrap`, `psx-bootstrap-project`).
- `/psx-bootstrap` then **copies** `tools/`, `.pi/extensions`, `.pi/skills`,
  `prompts/reference`, the docs and the vendor sources into the project
  (`tools/build/scaffoldProject.ts:166-170`, `docs/bootstrap.md`).
- The reason: every tool derives its root from its own file location. Examples
  are `decompToolchain.ts:27` `ROOT`, `toolchain.ts:9` `HARNESS_ROOT` and
  `pipeline.ts`. The extensions also recognise a project only when it contains
  `AGENTS.md` and `tools/agent/`, and they launch tools as `npx tsx tools/...`
  with the cwd set to the project.

So after migration this repository would still contain the same paths, written
by the harness instead of edited here. There is also no upgrade path.
- Bootstrap treats any existing file that differs as a conflict, writes
  nothing, and has no force option (`scaffoldProject.ts:154`, and the
  `applyScaffold` refusal).
- So once a newer harness changes a file, a second bootstrap over this
  repository's copy refuses. From then on the copy is a fork, which is the
  problem the migration is meant to end.

**Decision 1 (owner): which end state.**
- **A. Snapshot.** Keep the copy model, and add an upgrade path. Bootstrap
  records a manifest of each file it wrote, with the package hash. An update
  replaces files whose project hash still equals the recorded hash, and
  reports the rest as local edits. Tool changes are made in the harness only.
- **B. Runtime package.** Finish the harness's P0 project-context work
  (`docs/game-agnosticity.md`, P0 rows 1–3). Tools then resolve the project
  root explicitly and run from the package, and this repository holds no tool
  sources.
  - This is the stated goal.
  - The package must then expose the matching extensions directly, and the
    project must not also carry copies.

With either option, this repository stops editing tooling from now on. The
pending `plans/owning-pass-routing-and-context-budget.md` should be implemented
in the harness.

## What moves and what stays

| Owner | Paths |
|---|---|
| Harness (deleted here) | `.pi/extensions/`, `.pi/skills/`, `tools/{agent,build,diagnostics,lib}`, `prompts/` (reference sheets, legacy templates, README), `test-fixtures/`, `tools/vendor/` and the four submodule registrations in `.gitmodules`, the `Makefile` |
| Harness (platform headers, kept here by bootstrap) | `include/{include_asm.h,macro.inc,labels.inc,gte_macros.inc,debughook.h,scratchpad.h}`, `include/psyq/*` (identical in both today) |
| This repository | `src/`, `configs/` (splat, symbols, `overlays.json`, `project-info.json`, `project-profile.md`, reconstruction and search specs), `include/{common.h,globals.h,globals_override.h,game_types.h,functions.h,sdk_types.h,overlays/*}`, `notes/`, `plans/`, `.pi/autoloop.json`, `.pi/settings.json`, `AGENTS.md`, `lib/`, `extracted/`, `run_output/` |
| New here | `toolchain.json`, `project.json` (`{"target":{"kind":"linked-image"}}`), npm build scripts |
| Deleted outright | `configs/flag_overrides.mk` (after translation), `tools/vendor/{psyq_sdk,homebrew-psyq,silent-hill-decomp}` (no build consumers; `homebrew-psyq` is a gitlink with no `.gitmodules` entry), `include/variables.h` (nothing includes it) |

`include/common.h` is this repository's policy (`CAPTURE_PREV_RET`) under a
platform name. Bootstrap keeps this repository's copy, and it should stay that
way.

Some things that worked against this repository's earlier fears do carry over
cleanly:
- The autoloop config and state code is byte-identical: `shared/config.ts`,
  `autoloop/{config,types,state}.ts`.
- So `.pi/autoloop.json` (52 allowlist entries, the ladder, commit flags) and
  `run_output/autoloop/state.json` (the parked map) carry over unchanged.
- The experiment-ledger code is unchanged.
- `fileGroupings.ts` and the citations in `triage.ts` are unchanged and still
  resolve here.
- The one tooling commit made here after the harness copy, `fdedbe9f`, is
  already in the harness.

## Sharp edges, most dangerous first

1. **Overlay `-G0` and the 22 per-file overrides are a silent codegen change.**
   - The `Makefile` swaps `-G8`→`-G0` for every `src/overlays/` TU and for
     the overlay assembler and maspsx flags (`Makefile:15-27,40-47`). It
     appends `CC1FLAGS_<stem>` from `configs/flag_overrides.mk`.
   - The harness has no automatic swap. It reads only
     `containerProfiles[id] ?? [kind]` (`toolchain.ts:86-99`) and `fileFlags`
     (`pipeline.ts:98`). The template sets both to `{}`.
   - With the template `toolchain.json`, every overlay compiles at `-G8` and
     every override is dropped. Builds, preparation and all diagnostics would
     report wrong verdicts with no error.
   - A leftover `flag_overrides.mk` would be ignored by the build but still
     audited by `source-policy.ts:534`.
   - The draft `toolchain.json` is in the appendix.
   - Translate from this repository, not from
     `examples/btn-decompilation/configs/flag_overrides.mk`. That copy is stale:
     it still lists `ovl_11_func_800FD034`.
2. **Flag-override authority is inverted.**
   - This repository's `AGENTS.md` and flags sheet let a worker land an
     override with its evidence comment and allowlist entry in the same change.
   - The harness hard-fails any worker edit to `toolchain.json`
     (`source-policy.ts:558`). JSON has no comment field, so the 22 evidence
     comments, and the overlay `-G0` evidence, have nowhere to live.
   - The harness also audits every `fileFlags` key repo-wide
     (`source-policy.ts:562-583`). Five existing overrides have no
     `flag-override` allowlist entry and would hard-fail:
     `ovl_21_func_800B90C4`, `ovl_11_func_800F8224`, `ovl_15_func_8013345C`,
     `ovl_11_func_800FE068`, `ovl_11_func_800F0E00`.

   **Decision 2 (owner):**
   - who may add an override;
   - where its evidence lives (a sidecar such as `configs/flag-evidence.md`
     keyed by stem, or the allowlist entry);
   - whether the five unlisted overrides get allowlisted or re-solved.

   Then update the flag bullet in `AGENTS.md` to match.
3. **The install model has no upgrade path** (Decision 1, above).
4. **`lib/` and the SDK paths.**
   - This repository tracks 2,170 converted objects in `lib/`, and the splat
     YAMLs reference 430 `../lib/...` paths.
   - The harness is removing its `lib/`, and the bootstrap ignore block adds
     `/lib/`. That does not untrack files, but any new converted object would
     be ignored.
   - `tools/vendor/psyq47/LIB` (the original SDK, 2,330 tracked files) sits
     inside `tools/`, which becomes harness territory. Bootstrap excludes it
     (`scaffoldProject.ts:97`).
   - Move it to a project-owned path such as `library-input/psyq/LIB`, and set
     `sdk.originalDir` to match.
   - The `PSYQ2ELF` environment fallback is gone (`splitSdkLibs.ts:111`). The
     converter path moves into `toolchain.json.tools.psyq2elf`, a tracked file,
     so a machine-specific path ends up in git.

   **Decision 3 (owner):** the `lib/` tracking policy and the SDK location.
5. **`.gitignore` hides the build pipeline from worktrees.**
   - This repository's unanchored `build/` rule plus the `tools/build/*`
     whitelist ignores a scaffolded `tools/build/pipeline.ts`
     (`git check-ignore` confirms it).
   - Autoloop workspaces are `git worktree add` from HEAD
     (`.pi/extensions/shared/workspace.ts:137`). They would lack the pipeline,
     and their split would fail.
   - Under Decision 1A, the copied tools must be committed and the old ignore
     rules replaced with the harness's anchored block.
   - Workspaces also symlink the local compiler builds from
     `tools/vendor/old-gcc/build-gcc-*-psx`. Whatever supplies cc1 after
     migration has to sit at the configured path in every workspace.
6. **The harness regex for `file-groupings.md` members regressed.**
   - `.pi/extensions/psx-decomp/autoloop/family.ts:76` changed
     `\((?:m|s|\?)\b` to `\((?:m|s|\?)\)`.
   - Against this repository's `notes/file-groupings.md`, recognised members
     drop from 1,566 to 656, because every `(m, matched …)` bullet stops
     matching. Park-time family inference weakens accordingly.
   - Fix it in the harness before migrating.
7. **Generated drift.**
   - The harness's `classifyGlobals.ts:96` scans `.text` to `layout.dataStart`
     instead of the hard-coded `0x80048190`, about 2.4 KB more. That can change
     `include/globals.h`, which every TU compiles against.
   - `genProjectProfile.ts` now reads `toolchain.json`, so
     `configs/project-profile.md` changes.
   - `build/sdkVersion.json` now needs `binarySha256`, so SDK detection returns
     null until `matchSignatures` reruns.
   - `npm run config-check` (`git diff --exit-code configs/ include/`) fails
     until these are regenerated. The regenerated `globals.h` must still build
     byte-identical before it is committed.
8. **Real-asset regression coverage disappears.**
   - Bootstrap copies no `*.test.ts` and skips the `test` script, so this
     repository's `npm test` globs would match nothing.
   - The harness re-fixtured its tests onto synthetic projects. That drops the
     assertions tied to this game:
     - the static-chain census and idempotence on `func_8001EAE4`,
       `ovl_11_func_800F5160` and `ovl_19_func_800B8E88`;
     - the branch-orientation fixtures for `ovl_11_func_8011E090`;
     - the ovl_25 dispatch table;
     - `contextExport` resolving every type in `functions.h`;
     - `family.test.ts` parsing the live `file-groupings.md`.

   **Decision 4 (owner):** where those assertions live. Options: a
   project-side suite run against the installed harness, or the harness's
   optional original-asset tests pointed at this repository.
9. **The verify gate gets slower and may time out.**
   - `psx_verify_build` and the finalize gate move from `make check` (EXE only,
     120 s) to `npm run check-all` (all 14 containers, 300 s;
     `verify-build.ts:13`, `gates.ts:168`).
   - The pipeline rebuilds the EXE for every overlay's engine export, serially:
     13 extra EXE passes.
   - Time it cold and warm. If it exceeds 300 s, the finalize gate fails on
     every function.
10. **Remaining `make` in the harness.**
    - `tools/agent/diffFunc.ts:183` still runs `make -j1`. Without a Makefile
      it quietly degrades to "BYTE VERIFY UNAVAILABLE".
    - Agent-facing text still says `make`:
      - `prompts/reference/stuck.md:24` (`make split`);
      - `prompts/reference/population.md:494` (`make check`);
      - `tools/agent/analyzeTargetLoopEmission.ts:71`;
      - `notes/tools-directory-structure.md`.
    - The comments in `worktree.ts:53,123` are stale; the code already runs
      npm.
    - This repository's `README.md` has 16 `make` references to rewrite.
11. **Lost project-specific citations (minor).**
    - `tools/diagnostics/webPartitionDiff.ts` replaced eight precedent
      citations with generic sheets. Those approvals and notes still exist
      here.
    - `fingerprintWebPartition.ts --audit` now audits every register-asm
      allowlist entry, and `func_80020e38` drops out.
    - The `flagProbe` and `compiler-oracle` citations to the FF98 retro and
      `period-idiom-priors` are gone.
    - Accept these, or have the harness read an optional project citation
      registry.
12. **Caches go cold once.**
    - Preparation packets, the preprocessed cache, idiom-corpus fingerprint
      IDs, `toolchain-inputs.stamp` and verification receipts all key on
      toolchain identity, which now hashes `toolchain.json` instead of the
      `Makefile`.
    - The first run after cutover recomputes everything. Old ledger rows stay
      readable but will not compare equal to new ones.
13. **Pi packaging details.**
    - The harness lists `@earendil-works/pi-coding-agent` and `pi-ai` 0.83.0 as
      dependencies, while the installed pi is 0.85.1.
    - Pi's packaging docs say to list core packages as `peerDependencies: "*"`.
      Otherwise the bootstrap extension's `withFileMutationQueue` comes from a
      second module copy, whose queue pi's own edit tool doesn't share.
    - If Decision 1B ever exposes the matching extensions from the package, the
      project must not also carry copies. Pi keeps the first tool registered
      under a name and silently drops the other (`ExtensionRunner.
      getAllRegisteredTools`). Event handlers and gates from both copies would
      still run.

## Execution sequence

**Phase 0. Harness preconditions.** Done in the harness repository; they
block the cutover.

- Decision 1, and either the upgrade manifest (A) or the project-context
  runtime (B).
- Decision 2 implemented in `source-policy.ts`, the flags sheet and the skills.
- Fix the `family.ts:76` regex.
- Replace the `make` calls in `diffFunc.ts` and the reference sheets.
- Core pi packages become `peerDependencies`.
- Implement the owning-pass routing plan there.

**Phase 1. Freeze and record a baseline here.**
- No more tooling edits in this repository.
- Run `make check-all` (green) and record:
  - the effective cc1, as and maspsx argv for every object (`make -n -B`);
  - object section hashes;
  - linker scripts and the generated `engine_syms`, `dep_syms`, `lib_bss_syms`
    and `undefined_*` files;
  - `include/globals.h`.
- Back up `run_output/autoloop/`.

**Phase 2. Rehearsal in a throwaway worktree or clone, with `extracted/`
symlinked.**

1. Delete the machinery listed under "What moves and what stays".
2. `pi install -l /abs/path/to/psx-decompilation-harness`, then
   `/psx-bootstrap exe`. The preflight must show no conflicts.
3. Add `toolchain.json` (appendix) and `project.json`. Relocate `psyq47`.
   Replace `.gitignore` with the harness block, keeping `lib/` tracked.
4. Equivalence checks. All must pass:
   - The effective argv per object equals the Phase 1 record, apart from
     absolute `-I` paths.
   - `npm run config-check` leaves `configs/` and `include/` unchanged except
     `project-profile.md`. If `globals.h` changes, prove the build still
     matches before accepting it.
   - Generated linker inputs and object sections equal the baseline.
   - Cold run: `rm -rf build`, `npm run split-all`, `npm run check-all`. All
     14 containers OK, and the rodata check passes.
   - Negative controls: removing one `fileFlags` entry, or the overlay profile,
     makes `check-all` fail.
   - `check-all` finishes inside the 300 s gate, cold and warm.
5. Autoloop smoke test:
   - an autoloop run of 2–3 functions, through workspace creation, matching,
     the finalize gate and a commit;
   - one forced park, confirming the existing parked map is honoured.

**Phase 3. Policy and documentation here.**
- `AGENTS.md`: the flag bullet (per Decision 2), the routes into the
  `Makefile`, and the declarations section if header roles moved.
- `README.md`: replace `make` with npm commands.
- Carry the override evidence into its new home.
- Decide on the five unlisted overrides.

**Phase 4. Cutover on `main`, as reviewable commits.**
1. Delete the machinery, `Makefile`, `configs/flag_overrides.mk`, the
   submodule registrations and the unused vendor trees.
2. Install and bootstrap.
3. Commit the copied files under Decision 1A, plus `toolchain.json`,
   `project.json` and the new ignore block.
4. Regenerate and commit the generated drift after byte verification.

Keep the `Makefile` until step 4 of Phase 2 passes. Rollback is reverting
these commits.

**Phase 5. After cutover.**
- Run one full autoloop and compare it with the 2026-10-10 run, using the
  measures in `plans/owning-pass-routing-and-context-budget.md`.
- Match rate and minutes per function should not regress.

## Appendix: draft `toolchain.json` deltas

Start from the harness template (`templates/project/toolchain.json`). Set the
three GCC `capabilities`, or RTL dumps, `-dp` and implicit-declaration checks
refuse to run. Then change:

```json
"profiles": { "overlay-g0": {
  "cc1": ["-O2","-G0","-mips1","-mcpu=r3000","-funsigned-char","-fpeephole","-ffunction-cse","-fpcc-struct-return","-fcommon","-fverbose-asm","-msoft-float","-mgas","-fgnu-linker","-quiet"],
  "as":  ["-march=r3000","-mtune=r3000","-EL","-G0","-no-pad-sections"] } },
"containerProfiles": { "overlay": "overlay-g0" },
"fileFlags": {
  "func_800231E8": ["-mno-split-addresses"], "func_80023060": ["-mno-split-addresses"],
  "func_80014494": ["-fno-cse-skip-blocks"], "func_80018B98": ["-fno-gcse"],
  "func_80022794": ["-fno-rerun-cse-after-loop"],
  "func_8002495C": ["-fno-schedule-insns","-fno-schedule-insns2"],
  "func_8002470C": ["-fno-schedule-insns","-fno-schedule-insns2"],
  "ovl_11_func_800F3D40": ["-fno-gcse"], "ovl_11_func_800F00E4": ["-fno-gcse"],
  "ovl_11_func_80103B24": ["-fno-cse-skip-blocks"], "ovl_21_func_800B98CC": ["-fno-schedule-insns"],
  "ovl_21_func_800BA698": ["-fno-gcse"], "ovl_17_func_800B9158": ["-fno-cse-skip-blocks"],
  "ovl_11_func_800E4BA4": ["-fno-rerun-loop-opt"], "ovl_21_func_800B90C4": ["-fno-schedule-insns"],
  "ovl_11_func_800F1678": ["-fno-cse-skip-blocks"], "ovl_11_func_800F8224": ["-fno-strength-reduce"],
  "ovl_15_func_8013345C": ["-fno-gcse"], "ovl_11_func_800FE068": ["-fno-strength-reduce"],
  "ovl_11_func_800F0E00": ["-fno-strength-reduce"], "ovl_11_func_800E7660": ["-fno-cse-skip-blocks"],
  "ovl_11_func_800EF8BC": ["-fno-cse-skip-blocks"] },
"sdk": { "version": "470", "convertedDir": "lib", "originalDir": "library-input/psyq/LIB",
         "signatures": "tools/vendor/psx_psyq_signatures/470" }
```

The maspsx flags are the same for overlays and the EXE (`Makefile:45`), so the
overlay profile replaces only `cc1` and `as` (`Makefile:44,46`). Regenerate
the `fileFlags` list from `configs/flag_overrides.mk` at cutover time; the list
above was taken on 2026-10-10.
