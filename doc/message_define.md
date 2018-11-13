### 用户消息定义

```json
{
    'sendid':11111,
    'recvid':22222,
    'type':0,
    'session':1,
    'ver':1,
    'uuid':0,
    'time':1542104716,
    'data':{}
}
```

**字段说明**

| 字段    | 类型   | 说明                                          |
| ------- | :----- | --------------------------------------------- |
| sendid  | number | 发送者的用户id                                |
| recvid  | number | 接受者的id(根据session来判断是用户id还是群id) |
| session | number | 会话类型 1单聊会话 2群聊会话 3系统会话        |
| type    | number | 消息类型，详细见消息类型定义                  |
| ver     | number | 消息的版本                                    |
| uuid    | number | 消息的uuid，由收到的消息服务器产生            |
| time    | number | 消息产生的unix时间戳                          |
| data    | object | 消息的具体内容，根据type的不同里面也不同      |

**消息类型**

| 消息类型 | 消息定义 | 说明                                                         |
| -------- | -------- | ------------------------------------------------------------ |
| 0        | 默认值   | 无意义                                                       |
| 1        | 文本消息 | 发送文本内容，文本统一转换为utf-8格式，支持emoji表情         |
| 2        | 图片消息 | 发送图片，这里发送是图片的链接，然后用链接去图片服务器取数据 |
| 3        | 音频消息 | 发送语音文件。包含文件的下载地址md5 文件后缀，音频的时长     |
| 4        | 视频消息 | 发送视频文件，包含文件的下载地址md5 文件后缀，视频的时长     |
| 5        | 文件消息 | 发送文件，这里发送是文件的链接，然后用链接去文件服务器取数据 |
| 6        | 表情消息 | 表情的索引，如果本地没有则用gif文件下载地址                  |
| 7        | 位置消息 | 包含位置的经纬度                                             |
| 8        | 名片消息 | 包含用户的名片信息                                           |



**消息体内容定义**

1.***文本消息***

```json
{
    data：{
        [
            'content':"hello world"
    		'refer':[111,111,111,111]
        ]
    },
}
```

data content消息的内容,refer这条消息提及的用户id

2.***图片消息***

```json
{
    data：{
        'url':"http://xxx.xx/",
    	'md5':"e10adc3949ba59abbe56e057f20f883e",
    	'sfix':".png",
    	'size':1113232,
    },
}
```
