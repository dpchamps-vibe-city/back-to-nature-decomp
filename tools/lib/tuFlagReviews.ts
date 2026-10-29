/** Explicitly reviewed whole-TU flag migrations, not module-wide construct grants.
 * The review binds the source path, original members, unchanged setting and
 * originating function exception. Renames do not change original identities.
 * Byte verification remains mandatory; this record is permission, not proof.
 */
import { existsSync, readFileSync } from "node:fs";
import { join } from "node:path";
import { ROOT } from "./psxExeInfo.js";

export const TU_FLAG_REVIEWS = "configs/tu_flag_reviews.json";
export interface TuFlagReview {
  sourcePath: string;
  container: string;
  members: string[];
  owner: string;
  flags: string;
}

export function originalIdentity(container: string, vram: number): string {
  return `${container}:0x${vram.toString(16).toUpperCase().padStart(8, "0")}`;
}

/** No make expansion, continuation, duplicate assignment or non-option token. */
export function literalFlagSetting(text: string, key: string): string | undefined {
  const lines = text.split("\n").filter(line =>
    line.match(/^\s*(?:(?:override|export|private)\s+)*CC1FLAGS_([A-Za-z0-9_]+)\b/)?.[1] === key);
  if (lines.length !== 1) return undefined;
  const flags = lines[0]!.match(/^CC1FLAGS_\S+?\s*:?=\s*(.*?)\s*$/)?.[1];
  return flags && /^(?:-[A-Za-z0-9_=+.,-]+)(?:\s+-[A-Za-z0-9_=+.,-]+)*$/.test(flags)
    ? flags.split(/\s+/).join(" ") : undefined;
}

export function parseTuFlagReviews(text: string | null): TuFlagReview[] {
  if (text === null) return [];
  const data = JSON.parse(text);
  const fail = (): never => { throw new Error(`${TU_FLAG_REVIEWS}: invalid or duplicate review`); };
  if (!data || data.schemaVersion !== 1 || !Array.isArray(data.reviews) ||
      Object.keys(data).some(key => !["schemaVersion", "reviews"].includes(key))) fail();
  const paths = new Set<string>();
  for (const r of data.reviews) {
    if (!r || typeof r !== "object" || Object.keys(r).filter(key => key !== "evidence").sort().join(",") !== "container,flags,members,owner,sourcePath" ||
        typeof r.container !== "string" || !/^[a-zA-Z0-9_]+$/.test(r.container) ||
        typeof r.sourcePath !== "string" || !/^src\/(?:overlays\/[a-zA-Z0-9_]+\/)?modules\/[a-z][a-z0-9_]*\.c$/.test(r.sourcePath) ||
        paths.has(r.sourcePath) || typeof r.flags !== "string" ||
        literalFlagSetting(`CC1FLAGS_review := ${r.flags}`, "review") !== r.flags ||
        !Array.isArray(r.members) || r.members.length < 2 || new Set(r.members).size !== r.members.length ||
        !r.members.every((id: unknown) => typeof id === "string" && id.startsWith(`${r.container}:`) && /^[a-zA-Z0-9_]+:0x[0-9A-F]{8}$/.test(id)) ||
        !r.members.includes(r.owner)) fail();
    paths.add(r.sourcePath);
  }
  /* Historical snapshots may carry documentary metadata; it has no bearing
     on the exact source/member/flag scope checked here. */
  return data.reviews.map(({ sourcePath, container, members, owner, flags }: TuFlagReview) =>
    ({ sourcePath, container, members, owner, flags }));
}

export function loadTuFlagReviews(root = ROOT): TuFlagReview[] {
  const path = join(root, TU_FLAG_REVIEWS);
  return parseTuFlagReviews(existsSync(path) ? readFileSync(path, "utf8") : null);
}

export function reviewedTuFlagOwner(reviews: readonly TuFlagReview[], sourcePath: string,
  container: string, members: readonly string[], flags: string | undefined): string | undefined {
  const review = reviews.find(r => r.sourcePath === sourcePath && r.container === container);
  if (!review || flags !== review.flags || members.length !== review.members.length ||
      new Set(members).size !== members.length || !members.every(id => review.members.includes(id))) return undefined;
  return review.owner;
}
