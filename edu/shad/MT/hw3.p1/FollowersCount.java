package twitter;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
// import org.apache.hadoop.mapreduce.lib.chain.ChainReducer;
import org.apache.hadoop.mapred.lib.ChainReducer;
import org.apache.hadoop.mapred.lib.ChainMapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class FollowersCount extends Configured implements Tool {

    public static class CountFollowersMapper extends
            Mapper<Text, Text, IntWritable, IntWritable> {

        private final static IntWritable one = new IntWritable(1);

        @Override
        public void map(Text key, Text value, Context context)
                throws IOException, InterruptedException {
            context.write(new IntWritable(Integer.parseInt(key.toString())), one);
        }
    }

    public static class IntSumReducer extends
            Reducer<IntWritable, IntWritable, IntWritable, IntWritable> {

        @Override
        public void reduce(IntWritable key, Iterable<IntWritable> values, Context context) 
                throws IOException, InterruptedException {
            int sum = 0;
            for (IntWritable val : values) {
                sum += val.get();
            }
            context.write(key, new IntWritable(sum));
        }
    }

    @Override
    public int run(String[] args) throws Exception {
        if (args.length != 2) {
            System.err
                    .println("Usage: twitter.FollowersCount <input_path> <output_path>");
            return -1;
        }

        /*
        Job job = new Job(getConf);
        job.setJarByClass(FollowersCount.class);
        job.setJobName("FollowersCount");
        */

        JobConf conf = new JobConf(getConf(), FollowersCount.class);
        conf.setJobName("FollowersCount");

        // job.setMapperClass(CountFollowersMapper.class);
        // job.setCombinerClass(IntSumReducer.class);
        // job.setReducerClass(IntSumReducer.class);
        ChainMapper.addMapper(conf, CountFollowersMapper.class,
                Text.class, Text.class, IntWritable.class, IntWritable.class,
                false, new JobConf(false));

        ChainReducer.setReducer(conf, IntSumReducer.class,
                IntWritable.class, IntWritable.class,
                IntWritable.class, IntWritable.class,
                true, new JobConf(false));

        // job.setMapOutputKeyClass(IntWritable.class);
        // job.setMapOutputValueClass(IntWritable.class);
        // job.setOutputKeyClass(IntWritable.class);
        // job.setOutputValueClass(IntWritable.class);

        // job.setInputFormatClass(KeyValueTextInputFormat.class);
        // job.setOutputFormatClass(TextOutputFormat.class);
        conf.setInputFormat(KeyValueTextInputFormat.class);
        conf.setOutputFormat(TextOutputFormat.class);

        // FileInputFormat.addInputPath(job, new Path(args[0]));
        // FileOutputFormat.setOutputPath(job, new Path(args[1]));

        FileInputFormat.setInputPath(conf, new Path(args[0]));
        FileOutputFormat.setOutputPath(conf, new Path(args[1]));

        // boolean success = job.waitForCompletion(true);
        // return success ? 0 : 1;

        JobClient.runJob(conf);
        return 0;
    }

    public static void main(String[] args) throws Exception {
        int ret = ToolRunner.run(new FollowersCount(), args);
        System.exit(ret);
    }

}
