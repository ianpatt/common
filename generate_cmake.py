import os

HEADER_TYPES = (".h", ".hpp", ".hxx", ".inl")
SOURCE_TYPES = (".c", ".cpp", ".cxx")
ALL_TYPES = HEADER_TYPES + SOURCE_TYPES

def main():
	cur = os.path.dirname(os.path.realpath(__file__))
	os.chdir(cur)

	tmp = list()
	with os.scandir("common") as it:
		for entry in it:
			if not entry.name.startswith('.') and entry.is_file():
				tmp.append(entry.name)

	headers = list()
	sources = list()
	for file in tmp:
		name = "common/" + file
		if name.endswith(HEADER_TYPES):
			headers.append(name)
		elif name.endswith(SOURCE_TYPES):
			sources.append(name)

	headers.sort()
	sources.sort()

	def do_make(a_filename, a_varname, a_files):
		out = open("cmake/" + a_filename + ".cmake", "w", encoding="utf-8")
		out.write("set(" + a_varname + "\n")

		for file in a_files:
			out.write("\t" + file + "\n")

		out.write(")\n")

	do_make("headerlist", "headers", headers)
	do_make("sourcelist", "sources", sources)

if __name__ == "__main__":
	main()
