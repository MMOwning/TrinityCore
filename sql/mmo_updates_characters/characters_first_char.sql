CREATE TABLE IF NOT EXISTS `first_char` (
 `ID` int(10) unsigned NOT NULL auto_increment COMMENT 'ID',
  `GUID` int(10) unsigned NOT NULL COMMENT 'char guid',
  `account` int(10) unsigned NOT NULL COMMENT 'accountid',
  `time` int(30) unsigned NOT NULL COMMENT 'time',
  `guildid` int(30) unsigned NOT NULL COMMENT 'guildid',
  `ip` varchar(30)  NOT NULL COMMENT 'ip',
  PRIMARY KEY (`GUID`),
  KEY `ID`(`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='6_2';