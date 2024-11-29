const grpc=require('@grpc/grpc-js')
const message_proto=require('./proto')
const { v4: uuidv4 } = require('uuid');
const emailModule=require('./email')
const const_module = require("./const");
const redis_module=require('./redis');
const { errorMonitor } = require('nodemailer/lib/xoauth2');
async function GetVerifyCode(call,callback){
    console.log("email is",call.request.email)
    try{
        let query_res=await redis_module.GetRedis(const_module.code_prefix+call.request.email)
        console.log("query_res is ", query_res)
        let uniqueId = query_res;
        if(query_res == null){ //为空证明之前没有发送过验证码
            uniqueId=uuidv4()
            if(uniqueId.length>4){
                uniqueId=uniqueId.substring(0,4)
            }
                let bres=await redis_module.SetRedisExpire(const_module.code_prefix+call.request.email,uniqueId,600)
                if(!bres){
                    callback(null,{email:call.request.email,
                        error:const_module.Errors.RedisErr
                    });
                    return;
                }
            
        }

        let text_str='验证码为'+uniqueId+'三分钟内完成注册'
        //send mail
        let mailOptions={
            from:'t75234229@126.com',
            to:call.request.email,
            'subject':'注册验证码', 
            text:text_str
        };
        let send_res=await emailModule.SendMail(mailOptions)
        console.log("send res is ", send_res)
        //参数的callback函数，第一个参数是错误信息，第二个参数是返回值
        callback(null, { email:  call.request.email,
            error:const_module.Errors.Success
        }); 
    }catch(error){
        console.log("catch error is ", error)

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Exception
        }); 
    }
}
function main(){
    let server=new grpc.Server()
    server.addService(message_proto.EmailService.service,{GetVerifyCode:GetVerifyCode})//接口名，具体的接口
    //security credentials
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
        server.start()
        console.log('grpc server started')        
    })
}