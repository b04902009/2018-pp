# WordCount
>>
### task 1:
>
```
$ make
$ make test
```
>
### task 2:
>
```
$ hdfs dfs -copyFromLocal input wordcnt-input
``` 
修改Makefile第一行
```makefile
INPUT = /user/B04902009/wordcnt-input
```
修改後執行
```
$ make test
```
>>>
# Average
>>
### task1:
>
###### Mapper
```java
while (stk.hasMoreTokens()) {
    // TODO: Task I, transform a line into <Key, IntPair> as you wish
    String name = stk.nextToken();
    int score = Integer.parseInt(stk.nextToken());
    IntPair data = new IntPair(score, 1);
    output.collect(new Text(name), data);
}
```
>
###### Reducer
```java
throws IOException {
	// TODO: Task I, transform <Key, Iterator<IntPair>> into <Key, Double>
	double sum = 0, num = 0;
	while(values.hasNext()){
		IntPair data = values.next();
		sum += data.getFirst();
		num += data.getSecond();
	}
	output.collect(key, new DoubleWritable(sum/num));
}
```
>

### task2:
>
###### Combiner
```java
throws IOException {
	// TODO: Task II, collect the part of Iterator<IntPair>, 
	// 			and then store them into <Key, IntPair>
	int sum = 0, num = 0;
	while(values.hasNext()){
		IntPair data = values.next();
		sum += data.getFirst();
		num += data.getSecond();
	}
	output.collect(key, new IntPair(sum, num));
}
```
>
>
修改Makefile第一行
```
INPUT = /user/hadoop/avg-input/avg-input.large
```
修改後分別執行有/無combiner的版本
```
$ make
$ time make test
```
>
>
#### Result
>
  *(with combiner, input data: large)*
```
real	1m15.621s
user	0m16.128s
sys	    0m1.388s
```
  *(without combiner, input data: large)*
```
real	2m59.473s
user	0m16.580s
sys	    0m1.580s
```
