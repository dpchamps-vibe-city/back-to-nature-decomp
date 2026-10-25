/**
 * failureLines.ts — the lines of a failed build or check that name its cause.
 *
 * A failed `make` ends with the target it stopped on, after pages of
 * warnings, assembler notices and cache reports; the last lines of its output
 * name the target, not the cause. One check that covers several changes is
 * only as useful as its failure is attributable, so a gate reports what the
 * tools said went wrong: compiler and linker errors with their files, byte
 * mismatches, a tool's thrown error, the name and interface guards' findings,
 * and the make target that stopped.
 */

/** A line that is a diagnosis in its own right. */
const CAUSES: readonly RegExp[] = [
  /^\S+:\d+:(?:\d+:)? (?!warning\b)/, /* cc1/cpp/as: file:line: message, warnings excluded */
  /undefined reference to|multiple definition of|relocation truncated|cannot find|No such file or directory/,
  /^(?:MISMATCH|  original|  built):?/,
  /^(?:\w*Error|error): /,
  /^\s+(?:not reconciled|conflict|arity|implicit|restated|undetermined): /, /* interfaceAudit --check */
  /^interfaces: \d+ unit\(s\) are not reconciled/,
  /^(?:Obsolete aliases|Stale generated names|Obsolete allowlist key)/, /* canonicalNames --check headings */
  /^\S+: still inconsistent|attribution drift/,
  /^make(?:\[\d+\])?: \*\*\* .*(?:Error|Stop)/,
];

/** Lines that are never the cause. */
const NOISE = /MASPSX: Warning|: warning:|In function `|In file included from|cache hit|^Refreshed |^\s*$|^\s*\d+%\||it\/s\]/;

/**
 * The cause lines of `output`, in order and deduplicated, at most `limit`;
 * the canonicalNames listings keep the entries under their headings. When no
 * line is recognisably a cause, the last lines that are not noise.
 */
export function failureLines(output: string, limit = 12): string[] {
  const lines = output.split("\n");
  const found: string[] = [];
  const add = (line: string): void => {
    if (!found.includes(line)) found.push(line);
  };
  for (let index = 0; index < lines.length && found.length < limit; index++) {
    const line = lines[index]!.trimEnd();
    if (NOISE.test(line) || !CAUSES.some((pattern) => pattern.test(line))) continue;
    add(line);
    if (/^(?:Obsolete aliases|Stale generated names|Obsolete allowlist key)/.test(line)) {
      for (let entry = index + 1; entry < lines.length && /^ {2}\S/.test(lines[entry]!) && found.length < limit; entry++) add(lines[entry]!.trimEnd());
    }
  }
  if (found.length > 0) return found.slice(0, limit);
  return lines.map((line) => line.trimEnd()).filter((line) => !NOISE.test(line)).slice(-Math.min(limit, 8));
}
