ORANGE=\033[0;33m
NC=\033[0m
GREEN=\033[0;32m

how_to_run: generate
	@echo -e "${ORANGE}USAGE: ./app.exe pathToDataFile numberOfEntries [numbersToEvaluate f(X)]";
	@echo -e "	${GREEN}=>  ./app.exe ./data/romania.dat 6 3 4.5 2.4 9 0${NC}";

generate: main.c
	gcc -Wall main.c -o app.exe

clean:
	rm app.exe