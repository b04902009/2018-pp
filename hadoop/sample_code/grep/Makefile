PROJ=Grep
JarFile=$(PROJ).jar
MainFunc=edu.ntu.pp2011.$(PROJ)
LocalOutDir="/tmp/$$USER/$(PROJ)_output"
dfsinput=/user/input
dfsoutput=$(PROJ)-output

all: class jar run output clean

class:
	-rm -rf $(PROJ)
	mkdir -p $(PROJ)
	javac -classpath /opt/hadoop/hadoop-0.20.2-core.jar -d $(PROJ) $(PROJ).java

jar:
	-rm -f $(JarFile)
	jar cvf $(JarFile) -C $(PROJ) .

run:
	-hadoop fs -rmr $(dfsoutput)
	hadoop jar $(JarFile) $(MainFunc) $(dfsinput) $(dfsoutput)

output:
	rm -rf $(LocalOutDir)
	hadoop fs -get $(dfsoutput) $(LocalOutDir)
	cp $(LocalOutDir)/part-00000 ./out

clean:
	hadoop fs -rmr $(dfsoutput)

help:
	@echo "Usage:"
	@echo " make class   - Compile to class file."
	@echo " make jar     - Build Jar File."
	@echo " make clean   - Clean up Output directory on HDFS."
	@echo " make run     - Run your MapReduce code on Hadoop."
	@echo " make output  - Download output file."
	@echo " make help    - Show Makefile options."
	@echo " "
	@echo "Example:"
	@echo " make class && make jar && make run && make output && make clean"
