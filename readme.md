#   项目实现的功能
##	1、此为QT工程 
##	2、arduino调试MPU6050 or MPU6500(使用了MPU自带的DMP模块)，读取数据后通过串口上传给此上位机 
###

 * 如下为抓取到的一组数据：
 		`*quat\t0.74\t-1.21\t-1.65\t-0.94\teuler\t60.65\t8.89\t69.12\typr\t60.65\t122.89\t154.46\tareal\t19056\t-17592\t28436\taworld\t1419\t-23557\t-11534\t#`
	以 * 开头，以 # 结尾，中间数据通过\t隔离。
###目前此项目工程尚不完善###