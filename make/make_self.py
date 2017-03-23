with open("make/dependencies") as I:
    lines = [line for line in I.read().split("\n") if line !=
             "" and "#" not in line]


header = \
        """.PHONY: all clean get-deps test install uninstall self


INSTALL_PATH = /usr/bin/wolves
REQUIREMENTS = libsfml-dev
BIN = ./bin/main.bin

all: $(BIN)

self: make/dependencies make/make_self.py
\t\tpython3 make/make_self.py

clean:
\t\trm -rf bin obj && mkdir bin obj
get-deps:
		apt-get update
		apt-get install $(REQUIREMENTS)
test:
		echo "no tests for now"
install: $(BIN)
		cp $(BIN) $(INSTALL_PATH)
uninstall:
		rm -f $(INSTALL_PATH)
"""

obj = ""
files = set()

for line in lines:
    main, others = [i.strip() for i in line.split(":")]
    folder, name = main.split("/")
    files.add(name)

    obj += "./obj/{0}.o: ./src/{1}/{2}.cpp ./inc/{1}/{2}.hpp".format(
        name, folder, name)
    for other in [file for file in others.split(" ") if file.strip() != ""]:
        ofolder, oname = other.split("/")
        obj += " ./obj/{}.o".format(oname)
        files.add(oname)

    obj += "\n\t\tg++ -c -o ./obj/{0}.o ./src/{1}/{0}.cpp --std=c++11 -O2".format(
        name, folder)

    obj += "\n\n"

objlist = " ".join(["./obj/{}.o".format(file) for file in files])

obj += "\n$(BIN): " + objlist

obj += "\n\t\tg++ {} -o $(BIN) -lsfml-graphics -lsfml-window -lsfml-system".format(
    objlist)

with open("makefile", "w") as O:
    O.write(header + obj)
