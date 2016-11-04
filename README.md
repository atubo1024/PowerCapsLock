# PowerCapsLock

PowerCapsLock是一个用于增强`CapsLock`按键功能的小工具，初衷是为了让所有应用都能像Vim一样通过`H, J, K, L`来替代`Left, Down, Up, Right`四个方向键，而`CapsLock`键的位置很适合用做控制键，所以在保持了`CapsLock`原本功能的基础上，将其改造成了控制键，实现将`CapsLock + H/J/K/L`映射为四个方向键的功能，以及其他的一些实用的按键映射。

## 使用方法

* `CapsLock Down`, `CapsLock Up`: 切换大写锁定，和`CapsLock`原本的功能一致；
* `CapsLock Down` + `H/J/K/L`: 分别转换为`Left, Down, Up, Right`四个方向键的按键事件；
* `CapsLock Down` + `Space`: 转换为`Win` + `Space`，解决Windows10切换输入法时`Win`键不方便按到的问题；
* `CapsLock Down` + 其他按键：转换为`Ctrl` + 其他按键，即通过`CapsLock`实现了`Ctrl`键的功能；

## 编译方法

* 编译环境：`mingw` (`vs c++`环境下未验证)

* 编译方法：

  ```shell
  $ make
  OR
  $ scons
  ```

* `power_capslock.exe`为编译好的二进制文件，在Windows10下测试通过；

## 安装方法

* 运行`power_capslock.exe`即可，结束应用目前只能通过任务管理器结束进程或其他类似方法；
* 开机自运行需自行添加到计划任务或者修改注册表等；

## License

MIT