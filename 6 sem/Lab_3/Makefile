CC = gcc
CFLAGS = -Wall -Wextra -std=c23
SRC = main.c morse.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = morse_converter

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
    $(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

test: $(EXECUTABLE)
    echo ".... . .-.. .-.. ---  .-- --- .-. .-.. -.." | ./$(EXECUTABLE) > temp_output.txt; \
    act=$$(cat temp_output.txt); \
    exp=$$(cat $(TEST_OUTPUT)); \
    if [ "$$act" = "$$exp" ]; then \
        echo "Test passed"; \
    else \
        echo "Test failed. Exp: $$exp, got $$act"; \
    fi

clean:
    rm -f $(OBJ) $(EXECUTABLE)

.PHONY: all clean
