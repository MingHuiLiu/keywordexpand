all:
	g++ -o index test.cpp lz.cpp sever.cpp index.cpp ../commom/func.cpp ../wordseg/seg.cpp ../wordseg/hmm.cpp ../wordseg/tree.cpp ../wordseg/symbol.cpp -I ../../software/xapian/include/ -I /usr/include/mysql/ -I /data/qspace/seanxywang/software/include/  -L /usr/lib64/mysql/ -L /home/qspace/lib/ -lcurl -levent -l xapian -l mysqlclient
clean:
	$(RM) -rf index