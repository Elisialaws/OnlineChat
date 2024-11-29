const nodemailer=require('nodemailer');
const config_module=require('./config');
/**
 * create the email transport
 * secure represents whether the connection is encrypted
 */
let transport=nodemailer.createTransport({
    host:'smtp.126.com',
    port:465,
    secure:true,
    auth:{
        user:config_module.email_user,
        pass:config_module.email_pass
    }
})
/**
 * send mail
 * Promise 是一个表示异步操作的最终完成（或失败）及其结果值的对象。
Promise 有三种状态：pending（待定）、fulfilled（已满足）、rejected（已拒绝）。
Promise 的状态改变只有两种可能：从 pending 变为 fulfilled 和从 pending 变为 rejected。
通过.then() 方法注册回调函数，可以获取异步操作的消息。
.catch() 方法是.then(null, rejection) 的别名，用于指定发生错误时的回调函数。
正常情况下调用resolve()，异常情况下调用reject()。
std::future 是 C++11 中引入的，用于获取异步计算的结果的对象。
futures 通常与 std::async 结合使用，用于通过任务并发执行来获取结果。
 * @returns
 */
function SendMail(mailOptions_){
    //send mail with defined transport object,callback function
    //asynchronously send mail
    return new Promise(function(resolve,reject){
    transport.sendMail(mailOptions_,function(err,info){
        if(err){
            console.log(err)
            reject(err) 
        }else{
            console.log('Email sent:'+info.response)
            resolve(info.response)
        }
    });
})
}
module.exports.SendMail=SendMail