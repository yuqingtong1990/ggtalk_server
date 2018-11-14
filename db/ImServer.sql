CREATE DATABASE `ImServer` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
SET FOREIGN_KEY_CHECKS=0;
USE `ImServer`;
-- ----------------------------
-- 创建用户信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImUser`;
CREATE TABLE `ImUser` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '人员的id',
  `cellphone` varchar(20) NOT NULL COMMENT '用户的手机，考虑到有国外手机号长度为20',
  `email` varchar(100) NOT NULL DEFAULT '' COMMENT '用户的邮箱',
  `name` varchar(50) NOT NULL COMMENT '人员昵称',
  `nickname` varchar(50) NOT NULL,
  `passwd` varchar(20) NOT NULL DEFAULT '' COMMENT '用户密码不为空',
  `gender` smallint(6) DEFAULT '0' COMMENT '用户性别 0未知 1男 2女',
  `avatar_url` varchar(1024) DEFAULT NULL COMMENT '头像url',
  `birthday` date DEFAULT NULL,
  `sign_info` varchar(1024) DEFAULT NULL COMMENT '个性签名',
  `school` varchar(50) DEFAULT NULL COMMENT '学校',
  `hometown` varchar(50) DEFAULT NULL COMMENT '家乡',
  `career` varchar(50) DEFAULT NULL COMMENT '职业',
  `describe` text,
  `createtime` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updatetime` timestamp NULL DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`id`,`cellphone`,`email`,`name`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4;


-- ----------------------------
-- 创建好友信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImFriends`;
CREATE TABLE `ImFriends` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `smallId` int(11) unsigned NOT NULL COMMENT '用户A的id',
  `bigId` int(11) unsigned NOT NULL COMMENT '用户B的id',
  `status` tinyint(1) unsigned DEFAULT '0' COMMENT '0正常 1解除',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`id`),
  KEY `idx_smallId_bigId_status_updated` (`smallId`,`bigId`,`status`,`updated`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;


-- ----------------------------
-- 创建黑名单信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImBlacklst`;
CREATE TABLE `ImBlacklst` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `usrId` int(11) unsigned NOT NULL COMMENT '用户的id',
  `blackId` int(11) unsigned NOT NULL COMMENT '黑名单用户的id',
  `status` tinyint(1) unsigned DEFAULT '0' COMMENT '0正常 1解除',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间',
  PRIMARY KEY (`id`),
  KEY `idx_usrId_blackId_status_updated` (`usrId`,`blackId`,`status`,`updated`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;


-- ----------------------------
-- 创建好友申请表
-- ----------------------------
DROP TABLE IF EXISTS `ImFriendsApply`;
CREATE TABLE `ImFriendsApply` (
  `id` int(11) NOT NULL,
  `usrid` int(11) DEFAULT NULL COMMENT '被申请好友的用户id',
  `peerid` int(11) DEFAULT NULL COMMENT '申请好友的人的id',
  `peername` varchar(50) DEFAULT NULL COMMENT '申请好友的人的昵称',
  `status` tinyint(4) DEFAULT NULL COMMENT '状态 1 未处理 2同意 3 不同意 4忽略',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- ----------------------------
-- 创建群信息信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImGroupInfo`;
CREATE TABLE `ImGroupInfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '群组信息表',
  `name` varchar(256) COLLATE utf8mb4_bin NOT NULL DEFAULT '' COMMENT '群名称',
  `avatar` varchar(1024) COLLATE utf8mb4_bin NOT NULL DEFAULT '' COMMENT '群头像',
  `creator` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '创建者用户id',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '群组类型，1-公有群;2-私有群;3-讨论组',
  `userCnt` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '成员人数',
  `maxnum` int(11) unsigned DEFAULT NULL COMMENT '最大的成员数 如果没有设置 则最大人数为500',
  `jointype` tinyint(3) NOT NULL DEFAULT '0' COMMENT '加入的策略\r\n ',
  `status` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '是否删除,0-正常，1-解散',
  `version` smallint(11) unsigned NOT NULL DEFAULT '1' COMMENT '群版本号',
  `lastChated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '最后聊天时间',
  `notice` varchar(255) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '群公告',
  `introduce` varchar(512) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '群介绍',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_name` (`name`(191)),
  KEY `idx_creator` (`creator`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin COMMENT='IM群信息';


-- ----------------------------
-- 创建群成员信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImGroupMember`;
CREATE TABLE `ImGroupMember` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '群成员表',
  `groupId` int(11) unsigned NOT NULL COMMENT '群Id',
  `userId` int(11) unsigned NOT NULL COMMENT '用户id',
  `status` tinyint(4) unsigned NOT NULL DEFAULT '1' COMMENT '是否退出群，0-正常，1-已退出',
  `identity` tinyint(4) DEFAULT NULL COMMENT '0:创建者 1:管理员 2:普通成员',
  `group_name` varchar(50) DEFAULT NULL COMMENT '群昵称(默认等于用户昵称)',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `idx_groupId_userId_status` (`groupId`,`userId`,`status`),
  KEY `idx_userId_status_updated` (`userId`,`status`,`updated`),
  KEY `idx_groupId_updated` (`groupId`,`updated`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='群成员的关系表';

-- ----------------------------
-- 创建群邀请信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImGroupInvite`;
CREATE TABLE `ImGroupInvite` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id 主键自增',
  `usrId` int(11) NOT NULL COMMENT '邀请的用户的id',
  `groupId` int(11) NOT NULL COMMENT '被邀请的群的id',
  `group_name` varchar(256) DEFAULT NULL COMMENT '被邀请加入的群的名词',
  `Inviterid` int(11) NOT NULL,
  `Invitername` varchar(255) DEFAULT NULL COMMENT '邀请人的昵称',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- 创建群申请信息表
-- ----------------------------
DROP TABLE IF EXISTS `ImGroupApply`;
CREATE TABLE `ImGroupApply` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id 主键自增',
  `usrId` int(11) NOT NULL COMMENT '申请的用户的id',
  `groupId` int(11) NOT NULL COMMENT '申请的群的id',
  `group_name` varchar(256) DEFAULT NULL COMMENT '被邀请加入的群的名词',
  `status` tinyint(4) NOT NULL COMMENT '状态 1 未处理 2同意 3 不同意 4忽略',
  `applyreason` varchar(100) DEFAULT NULL COMMENT '加群理由',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- ----------------------------
-- 创建好友添加策略表
-- ----------------------------
DROP TABLE IF EXISTS `ImAddFriendStrategy`;
CREATE TABLE `ImAddFriendStrategy` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'id 主键自增',
  `usrId` int(11) NOT NULL COMMENT '申请的用户的id',
  `strategy` tinyint(4) NOT NULL COMMENT '添加策略 1 同意任何用户加好友 2 拒绝任何人加好友 3 需要验证',
  `created` timestamp NULL DEFAULT NULL COMMENT '创建时间',
  `updated` timestamp NULL DEFAULT NULL COMMENT '更新时间'ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`,`usrId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

