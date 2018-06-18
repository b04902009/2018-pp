package edu.ntu.pp2011;

import java.io.IOException;
import java.util.*;
import java.lang.StringBuilder;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

public class Grep {
    public static class Map extends MapReduceBase 
	implements Mapper<LongWritable, Text, Text, Text> {
	
        public void map(LongWritable key, Text value, 
			OutputCollector<Text, Text> output, Reporter reporter) 
	    throws IOException {
	    FileSplit fileSplit = (FileSplit)reporter.getInputSplit();
	    String line = value.toString();
	    StringTokenizer stk = new StringTokenizer(line.toLowerCase());
	    while(stk.hasMoreTokens()){
		String word = stk.nextToken();
		output.collect(new Text(word), 
			       new Text(fileSplit.getPath().getName() + ":" + line));
	    }
        }
    }

    public static class Reduce extends MapReduceBase 
	implements Reducer<Text, Text, Text, Text> {
        public void reduce(Text key, Iterator<Text> values, 
			   OutputCollector<Text, Text> output, Reporter reporter) 
	    throws IOException {
	    StringBuilder sb = new StringBuilder("\n");
	    while(values.hasNext()){
		String v = values.next().toString().trim();
		sb.append(v + "\n");
	    }
	    output.collect(key, new Text(sb.toString()));
        }
    }

    public static void main(String[] args) throws Exception {
        JobConf conf = new JobConf(Grep.class);
        conf.setJobName("Grep");

        conf.setOutputKeyClass(Text.class);
        conf.setOutputValueClass(Text.class);

        conf.setMapOutputKeyClass(Text.class);
        conf.setMapOutputValueClass(Text.class);

        conf.setMapperClass(Map.class);
        conf.setCombinerClass(Reduce.class);
        conf.setReducerClass(Reduce.class);

        conf.setInputFormat(TextInputFormat.class);
        conf.setOutputFormat(TextOutputFormat.class);

        FileInputFormat.setInputPaths(conf, new Path(args[0]));
        FileOutputFormat.setOutputPath(conf, new Path(args[1]));

        JobClient.runJob(conf);
    }
}

