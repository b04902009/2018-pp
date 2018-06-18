package edu.ntu.pp2011;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

public class Hello {

    public static class Map extends MapReduceBase 
	implements Mapper<LongWritable, Text, Text, NullWritable> {
    	public void map(LongWritable key, Text value, 
			OutputCollector<Text, NullWritable> output, Reporter reporter) 
	    throws IOException {
	    output.collect(value, NullWritable.get());
        }
    }

    public static class Reduce extends MapReduceBase 
	implements Reducer<Text, NullWritable, Text, NullWritable> {
        public void reduce(Text key, Iterator<NullWritable> values, 
			   OutputCollector<Text, NullWritable> output, Reporter reporter)
	    throws IOException {
		
	    while(values.hasNext()){
		output.collect(key, values.next());
	    }
        }
    }

    public static void main(String[] args) throws Exception {
        JobConf conf = new JobConf(Hello.class);
        conf.setJobName("Hello");

        conf.setOutputKeyClass(Text.class);
        conf.setOutputValueClass(NullWritable.class);

        conf.setMapOutputKeyClass(Text.class);
        conf.setMapOutputValueClass(NullWritable.class);

        conf.setMapperClass(Map.class);
        conf.setReducerClass(Reduce.class);

        conf.setInputFormat(TextInputFormat.class);
        conf.setOutputFormat(TextOutputFormat.class);

        FileInputFormat.setInputPaths(conf, new Path(args[0]));
        FileOutputFormat.setOutputPath(conf, new Path(args[1]));

        JobClient.runJob(conf);
    }
}

