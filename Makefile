all:
	g++ -o index test.cpp index.cpp ../commom/func.cpp ../wordseg/seg.cpp ../wordseg/hmm.cpp ../wordseg/tree.cpp ../wordseg/symbol.cpp -I ../../software/xapian/include/ -I /usr/include/mysql/ -L /usr/lib64/mysql/ -L /home/qspace/lib/ -l xapian -l mysqlclient
clean:
	$(RM) -rf index
