# maspsx upstream update

## 2026-10-16

- Previous submodule pin: `0249ed2752a100eed95e8b4124f94af859398ca9`
  (`Pass --check flag to black in CI action`, 2026-02-20).
- New pin: `e85ecb373828aabea96d7e6400974d4f91ce8c74`
  (`Add --max-comm-alignment (#151)`, 2026-10-02), the latest
  `mkst/maspsx` upstream `master` fetched for this update.
- The previous checkout was clean. No local patches are carried forward.
- Upstream [PR #143](https://github.com/mkst/maspsx/pull/143), commit
  `36299446e68e75d1559209a311fbc43270257651`, supplies the missing
  `$16`/`$17` and `$20`/`$21` register-pair mappings for `li.d`, among
  other missing pairs. The old pin fails to assemble
  `ovl_11_func_800EB328` without those mappings.
- Build configuration and ASPSX version selection are unchanged.

Verification: `make -j4 -k check-all` exited **0**. The PS-X EXE and all
**13 overlay containers** compile, link, and match their original bytes;
rodata attribution checks also pass. This includes `ovl_11_func_800EB328`.
The toolchain content stamp tracks maspsx source changes and forced a rebuild
with the new assembler. Full output is saved to
`build/maspsx-upgrade-check-all.log`.

The unrelated pre-existing edits to `include/globals_override.h` were preserved
and are not part of this update.
