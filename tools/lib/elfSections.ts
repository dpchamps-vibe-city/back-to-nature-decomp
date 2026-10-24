/** Strict ELF32 section-table reader. No host tools, no guessed offsets. */
export interface Elf32Section {
  index: number;
  type: number;
  offset: number;
  size: number;
  link: number;
  info: number;
  entsize: number;
  data: Buffer;
}

export function elf32Sections(bytes: Buffer): Map<string, Elf32Section> {
  const fail = (message: string): never => { throw new Error(`Invalid ELF32: ${message}`); };
  const extent = (offset: number, size: number) => {
    if (!Number.isSafeInteger(offset + size) || offset < 0 || size < 0 || offset + size > bytes.length) fail("section extent outside file");
  };
  extent(0, 52);
  if (bytes.toString("hex", 0, 4) !== "7f454c46" || bytes[4] !== 1 || ![1, 2].includes(bytes[5]!) || bytes[6] !== 1) fail("unsupported magic/class/encoding/version");
  const u16 = (offset: number) => bytes[5] === 1 ? bytes.readUInt16LE(offset) : bytes.readUInt16BE(offset);
  const u32 = (offset: number) => bytes[5] === 1 ? bytes.readUInt32LE(offset) : bytes.readUInt32BE(offset);
  if (u16(40) !== 52 || u32(20) !== 1) fail("invalid header");
  const table = u32(32), stride = u16(46);
  if (!table || stride !== 40) fail("missing/unsupported section table");
  extent(table, stride);
  const count = u16(48) || u32(table + 20);
  const namesIndex = u16(50) === 0xffff ? u32(table + 24) : u16(50);
  if (!count || namesIndex >= count) fail("invalid section count/string index");
  extent(table, stride * count);
  const namesHeader = table + namesIndex * stride;
  if (u32(namesHeader + 4) !== 3) fail("section names are not STRTAB");
  const names = u32(namesHeader + 16), namesSize = u32(namesHeader + 20);
  extent(names, namesSize);
  const result = new Map<string, Elf32Section>();
  for (let i = 0; i < count; i++) {
    const header = table + i * stride;
    const nameOffset = u32(header), type = u32(header + 4), size = u32(header + 20);
    if (type !== 0 && type !== 8) extent(u32(header + 16), size); /* NULL / NOBITS occupy no file bytes. */
    if (nameOffset >= namesSize) fail("invalid section name offset");
    const end = bytes.indexOf(0, names + nameOffset);
    if (end < 0 || end >= names + namesSize) fail("unterminated section name");
    const name = bytes.toString("utf8", names + nameOffset, end);
    if (name && result.has(name)) fail(`duplicate section ${name}`);
    if (name) result.set(name, {
      index: i, type, offset: u32(header + 16), size,
      link: u32(header + 24), info: u32(header + 28), entsize: u32(header + 36),
      data: type === 8 ? Buffer.alloc(0) : bytes.subarray(u32(header + 16), u32(header + 16) + size),
    });
  }
  return result;
}

export function elf32SectionSizes(bytes: Buffer): Map<string, number> {
  return new Map([...elf32Sections(bytes)].map(([name, section]) => [name, section.size]));
}

export interface Elf32Symbol {
  name: string;
  value: number;
  size: number;
  /** STT_*: 0 NOTYPE, 1 OBJECT, 2 FUNC, 3 SECTION, 4 FILE. */
  type: number;
  /** STB_*: 0 LOCAL, 1 GLOBAL, 2 WEAK. */
  bind: number;
  /** Name of the defining section; "" for undefined, absolute and common. */
  section: string;
}

/** The `.symtab` of a relocatable ELF32 object, read in-process. */
export function elf32Symbols(bytes: Buffer): Elf32Symbol[] {
  const sections = elf32Sections(bytes);
  const byIndex = new Map([...sections].map(([name, section]) => [section.index, { name, section }]));
  const table = sections.get(".symtab");
  if (!table) return [];
  if (table.entsize !== 16) throw new Error("Invalid ELF32: unsupported symbol entry size");
  const strings = byIndex.get(table.link)?.section;
  if (!strings || strings.type !== 3) throw new Error("Invalid ELF32: symbol names are not STRTAB");
  const little = bytes[5] === 1;
  const u16 = (data: Buffer, offset: number) => little ? data.readUInt16LE(offset) : data.readUInt16BE(offset);
  const u32 = (data: Buffer, offset: number) => little ? data.readUInt32LE(offset) : data.readUInt32BE(offset);
  const symbols: Elf32Symbol[] = [];
  for (let offset = 0; offset + 16 <= table.data.length; offset += 16) {
    const nameOffset = u32(table.data, offset);
    if (nameOffset >= strings.data.length) throw new Error("Invalid ELF32: symbol name offset");
    const end = strings.data.indexOf(0, nameOffset);
    if (end < 0) throw new Error("Invalid ELF32: unterminated symbol name");
    const info = table.data[offset + 12]!;
    const shndx = u16(table.data, offset + 14);
    symbols.push({
      name: strings.data.toString("utf8", nameOffset, end),
      value: u32(table.data, offset + 4),
      size: u32(table.data, offset + 8),
      type: info & 0xf,
      bind: info >> 4,
      section: shndx > 0 && shndx < 0xff00 ? byIndex.get(shndx)?.name ?? "" : "",
    });
  }
  return symbols;
}
