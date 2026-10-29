import assert from "node:assert/strict";
import test from "node:test";
import { literalFlagSetting, parseTuFlagReviews, reviewedTuFlagOwner, type TuFlagReview } from "./tuFlagReviews.js";

const review: TuFlagReview = {
  sourcePath: "src/modules/pair.c", container: "exe", members: ["exe:0x80010000", "exe:0x80010010"],
  owner: "exe:0x80010010", flags: "-fno-cse-skip-blocks",
};

test("whole-TU permission binds path, container, every original identity and exact options", () => {
  const rows = parseTuFlagReviews(JSON.stringify({ schemaVersion: 1, reviews: [review] }));
  const owner = (path = review.sourcePath, container = review.container, members = review.members, flags: string | undefined = review.flags) =>
    reviewedTuFlagOwner(rows, path, container, members, flags);
  assert.equal(owner(), review.owner);
  assert.equal(owner(review.sourcePath, "ovl_11"), undefined);
  assert.equal(owner("src/modules/other.c"), undefined);
  for (const members of [review.members.slice(0, 1), [...review.members, "exe:0x80010020"], [review.members[0]!, review.members[0]!]])
    assert.equal(owner(review.sourcePath, "exe", members), undefined);
  assert.equal(owner(review.sourcePath, "exe", review.members, "-fno-cse-skip-blocks -fno-gcse"), undefined);
  assert.equal(reviewedTuFlagOwner([], review.sourcePath, "exe", review.members, review.flags), undefined);
});

test("flag records need no document and ignore legacy evidence metadata", () => {
  const parse = (row: object) => parseTuFlagReviews(JSON.stringify({ schemaVersion: 1, reviews: [row] }));
  assert.deepEqual(parse(review), [review]);
  assert.deepEqual(parse({ ...review, evidence: "deleted/report.md#old" }), [review]);
  assert.deepEqual(parse({ ...review, evidence: "" }), [review]);
});

test("malformed, duplicate or unbounded reviews fail closed", () => {
  assert.deepEqual(parseTuFlagReviews(null), []);
  for (const changed of [
    { ...review, members: [review.owner] }, { ...review, owner: "exe:0x80010020" },
    { ...review, members: [...review.members, review.owner] }, { ...review, flags: "$(FLAGS)" },
    { ...review, constructs: ["embedded-asm"] },
    { ...review, sourcePath: "../pair.c" }, { ...review, members: ["ovl_11:0x80010000", review.owner] },
  ]) assert.throws(() => parseTuFlagReviews(JSON.stringify({ schemaVersion: 1, reviews: [changed] })));
  assert.throws(() => parseTuFlagReviews(JSON.stringify({ schemaVersion: 1, reviews: [review, review] })));
});

test("flag migration never evaluates make expressions or ambiguous assignments", () => {
  assert.equal(literalFlagSetting("CC1FLAGS_x := -O2   -fno-cse-skip-blocks\n", "x"), "-O2 -fno-cse-skip-blocks");
  for (const extra of ["CC1FLAGS_x += -fno-gcse", "override CC1FLAGS_x := -fno-gcse", "CC1FLAGS_x ?= -G0"])
    assert.equal(literalFlagSetting(`CC1FLAGS_x := -O2\n${extra}`, "x"), undefined);
  for (const text of ["CC1FLAGS_x := $(OTHER)", "CC1FLAGS_x := -O2 \\\n -G8", "CC1FLAGS_x := -O2\nCC1FLAGS_x := -G8", "CC1FLAGS_x := -O2 # hidden", "CC1FLAGS_y := -O2"])
    assert.equal(literalFlagSetting(text, "x"), undefined);
});
