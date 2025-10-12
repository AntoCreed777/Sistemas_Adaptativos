EXPERIMENT=experiments_taboo
SRC=$(EXPERIMENT).rs
OUT=$(EXPERIMENT).out

all: $(OUT)

$(OUT): $(SRC)
	rustc $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
