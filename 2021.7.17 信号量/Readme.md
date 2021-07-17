## 信号量应用



使用信号量让第一个文本框创建线程后自加到1000，然后再创建3个线程同时加到100，注意这里可以对函数进行封装然后多次调用



这里我使用的是`(HWND)lpParameter`这个方法传入`GetWindowText`，这里也可以用`hThreadArr[dwIndex]`传入



`lReleaseCount`这个参数设置为2，允许两个线程同时跑



实现效果如下



![semaphore](semaphore.gif)