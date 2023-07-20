mahimahi: a web performance measurement toolkit
编译方法和 mahimahi 官网相同
mm-delay 使用方法：

mm-delay 50 和原有的 mahimahi 相同。
可新增一个参数 mm-delay 50 10 10意味着波动值，每个包的 rtt 随机波动 50 + [-10,10] 
需注意第二个参数必须小于等于第一个参数。
