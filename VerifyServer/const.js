/**
 * module.exports
用途: CommonJS 规范中定义的导出机制，主要用于 Node.js 环境。
语法: 使用 module.exports 来导出一个对象、函数或变量。
导入: 使用 require 来导入模块。
特点:
可以导出单个值（对象、函数、类等）。
支持动态导出（可以在运行时修改导出的内容）。
 */
/**
 * export
用途: ES6 模块规范中定义的导出机制，主要用于浏览器和支持 ES6 的环境。
语法: 使用 export 或 export default。
导入: 使用 import 来导入模块。
特点:
可以导出多个值（命名导出）或单个默认值（默认导出）。
静态导入（在编译时确定依赖关系），这使得现代 JavaScript 引擎能够更好地优化代码。
 */
let code_prefix = "code_";

const Errors = {
    Success : 0,
    RedisErr : 1,
    Exception : 2,
};


module.exports = {code_prefix,Errors}