all:
	g++ -o index test.cpp index.cpp ../commom/func.cpp ../wordseg/seg.cpp ../wordseg/hmm.cpp ../wordseg/tree.cpp ../wordseg/symbol.cpp -I ../../software/xapian/include/ -L /home/qspace/lib/ -l xapian
clean:
	$(RM) -rf index
