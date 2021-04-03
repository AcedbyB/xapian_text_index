CC = g++ `xapian-config --libs --cxxflags`
OBJ = metadata_indexer

all: $(OBJ)
metadata_indexer: metadata_indexer.cc
	$(CC) -o $@ -c $<
