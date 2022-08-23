SRCCODE := main.c "Card/card.c" "helper/help.c" "Terminal/terminal.c" "Server/server.c" "Application\app.c"
OBJECT_FILE := application.o
EXECUTABLE := app.exe
CC := gcc

.PHONY: default
default: compile

.PHONY: compile
compile:
	@$(CC) $(SRCCODE) -o $(OBJECT_FILE)
	@$(CC) $(SRCCODE) -o $(EXECUTABLE)

	@echo "--------- Program Size ---------"
	@size $(OBJECT_FILE)

.PHONY: run
run: compile
	@echo "--------- Welcome Payment Application ---------"
	@$(EXECUTABLE)

.PHONY: clean
clean:
	@rm  -f *.exe *.o *.i *.s