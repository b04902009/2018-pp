# word count
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
# average
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
      int sum = 0, cnt = 0;
      while(values.hasNext()) {
          IntPair data = values.next();
          sum += data.getFirst();
          cnt += data.getSecond();
      }
      output.collect(key, new DoubleWritable((double)sum/cnt));
  }
  ```
>

### task2:
>
###### Combiner
  ```java
  throws IOException {
      // TODO: Task II, collect the part of Iterator<IntPair>, 
      //          and then store them into <Key, IntPair>
      int sum = 0, cnt = 0;
      while(values.hasNext()) {
          IntPair data = values.next();
          sum += data.getFirst();
          cnt += data.getSecond();
      }
      output.collect(key, new IntPair(sum, cnt));
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
  real    1m39.247s
  user    0m16.760s
  sys     0m1.828s
```
  *(without combiner, input data: large)*
```
  real    3m25.316s
  user    0m17.036s
  sys     0m1.780s
```
