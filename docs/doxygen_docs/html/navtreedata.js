/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Appetito", "index.html", [
    [ "Hopefully Useful Coding Standard", "md__coding__standard.html", [
      [ "Introduction", "md__coding__standard.html#autotoc_md1", null ],
      [ "观念", "md__coding__standard.html#autotoc_md2", [
        [ "P.1 能在代码里表现的思想不要靠注释来表现", "md__coding__standard.html#autotoc_md3", null ],
        [ "P.2 编译时能解决的错误不留到运行时，剩下的错误尽量在运行时都**尽早**捉到", "md__coding__standard.html#autotoc_md4", null ],
        [ "P.3 防止资源泄露", "md__coding__standard.html#autotoc_md5", null ],
        [ "P.4 尽量少浪费时间和空间", "md__coding__standard.html#autotoc_md6", null ],
        [ "P.5 封装贴近底层的、不直白的代码，并提供直白易用的接口", "md__coding__standard.html#autotoc_md7", null ]
      ] ],
      [ "接口", "md__coding__standard.html#autotoc_md8", [
        [ "I.1 接口应该直白，不应该有隐藏的条件，如果有条件应该明确写出", "md__coding__standard.html#autotoc_md9", null ],
        [ "I.2 尽量避免使用全局变量，尤其是非 const 的全局变量", "md__coding__standard.html#autotoc_md10", null ],
        [ "I.3 接口的类型应该明确清晰", "md__coding__standard.html#autotoc_md11", null ],
        [ "I.4 当程序发现无法完成某项任务时，应该抛出异常", "md__coding__standard.html#autotoc_md12", null ],
        [ "I.5 不要用指针作为函数的返回值", "md__coding__standard.html#autotoc_md13", null ],
        [ "I.6 不要用一个单独的指针传递数组，同时用一个单独的 n 把数组大小传进函数也不行", "md__coding__standard.html#autotoc_md14", null ],
        [ "I.7 尽量减少函数的参数个数", "md__coding__standard.html#autotoc_md15", null ]
      ] ],
      [ "函数", "md__coding__standard.html#autotoc_md16", [
        [ "F.1 尽量把重复的、有意义的操作“打包”成一个函数", "md__coding__standard.html#autotoc_md17", null ],
        [ "F.2 一个函数应该只进行**一个**有意义的操作", "md__coding__standard.html#autotoc_md18", null ],
        [ "F.3 一个函数应该比较短、比较简单", "md__coding__standard.html#autotoc_md19", null ]
      ] ],
      [ "类", "md__coding__standard.html#autotoc_md20", [
        [ "C.1 当类有一个不变的规则时用 class ， 当类的每一个成员都可以单独改变的时候用 struct", "md__coding__standard.html#autotoc_md21", null ],
        [ "C.2 接口用 public ， 实现用 private", "md__coding__standard.html#autotoc_md22", null ],
        [ "C.3 尽量少使用类的继承结构", "md__coding__standard.html#autotoc_md23", null ],
        [ "C.4 一定定义构造函数", "md__coding__standard.html#autotoc_md24", null ],
        [ "C.5 如果有一个指针成员，一定定义析构函数", "md__coding__standard.html#autotoc_md25", null ]
      ] ],
      [ "资源", "md__coding__standard.html#autotoc_md26", [
        [ "R.1 RAII", "md__coding__standard.html#autotoc_md27", null ],
        [ "R.2 尽量使用有作用域的对象，少使用动态内存分配", "md__coding__standard.html#autotoc_md28", null ],
        [ "R.3 动态内存分配，用 new 和 delete 而不是 malloc 和 free", "md__coding__standard.html#autotoc_md29", null ],
        [ "R.4 推荐：使用 span 来保存数组的大小", "md__coding__standard.html#autotoc_md30", null ]
      ] ],
      [ "表达式和语句", "md__coding__standard.html#autotoc_md31", [
        [ "ES.1 优先使用标准库，然后是第三方库，最后才是自己实现", "md__coding__standard.html#autotoc_md32", null ],
        [ "ES.2 合适地进行抽象", "md__coding__standard.html#autotoc_md33", null ],
        [ "ES.3 减少重复冗余的代码", "md__coding__standard.html#autotoc_md34", null ],
        [ "ES.4 一些命名建议", "md__coding__standard.html#autotoc_md35", null ],
        [ "ES.5 一次声明只声明一个变量名", "md__coding__standard.html#autotoc_md36", null ],
        [ "ES.6 一定记得初始化对象，最好在声明时就初始化", "md__coding__standard.html#autotoc_md37", null ],
        [ "ES.7 避免“魔法常量”", "md__coding__standard.html#autotoc_md38", null ],
        [ "ES.8 避免变量类型转换，特别是窄化的类型转换", "md__coding__standard.html#autotoc_md39", null ],
        [ "ES.9 空指针应该用 nullptr ， 不要用 0 或者 NULL", "md__coding__standard.html#autotoc_md40", null ],
        [ "ES.10 避免使用 do while 等 do 语句", "md__coding__standard.html#autotoc_md41", null ],
        [ "ES.11 尽量减少循环中 break 和 continue 的使用", "md__coding__standard.html#autotoc_md42", null ]
      ] ],
      [ "附加", "md__coding__standard.html#autotoc_md43", [
        [ "A.1 声明指针时，星号跟着类型名，而不是挨着变量名： int* p", "md__coding__standard.html#autotoc_md44", null ],
        [ "A.2 每个函数前面应该用注释注明它的基本信息", "md__coding__standard.html#autotoc_md45", null ]
      ] ]
    ] ],
    [ "文档", "md__xE6_x96_x87_xE6_xA1_xA3.html", null ],
    [ "🍜 吃了没 Appetito", "md__c___projects__appetito__r_e_a_d_m_e.html", [
      [ "Overview", "md__c___projects__appetito__r_e_a_d_m_e.html#autotoc_md48", null ],
      [ "Roadmap", "md__c___projects__appetito__r_e_a_d_m_e.html#autotoc_md49", null ],
      [ "TO-DO", "md__c___projects__appetito__r_e_a_d_m_e.html#autotoc_md50", null ],
      [ "Under construction 🚫", "md__c___projects__appetito__r_e_a_d_m_e.html#autotoc_md51", null ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_c_make_c_compiler_id_8c.html",
"class_table.html#ac5eda3657783ac573315904e9e9f951e"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';