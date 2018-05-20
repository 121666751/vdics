/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE */;
/*!40101 SET SQL_MODE='' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES */;
/*!40103 SET SQL_NOTES='ON' */;

DROP DATABASE IF EXISTS gsipdb;

CREATE DATABASE gsipdb;

grant all privileges on gsipdb.* to gsip@'%' identified by 'gsip0728';
grant all privileges on gsipdb.* to gsip@'localhost' identified by 'gsip0728';

USE gsipdb;

DROP TABLE IF EXISTS bocctrl;
CREATE TABLE bocctrl (
  exchno varchar(16) NOT NULL default '',
  date varchar(12) NOT NULL default '',
  session smallint(6) NOT NULL default '0',
  dramount decimal(14,2) default '0.00',
  drcount int(11) default '0',
  cramount decimal(14,2) default '0.00',
  crcount int(11) default '0',
  whamount decimal(14,2) default '0.00',
  whcount int(11) default '0',
  ccamount decimal(14,2) default '0.00',
  dcamount decimal(14,2) default '0.00',
  printed smallint(6) default '0',
  state varchar(16) default '000000000000000',
  area varchar(8) default '',
  checked int(4) default '0',
  PRIMARY KEY  (exchno,date,session,area),
  KEY bocctrl_exchno (exchno,date,session,area)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS bocnets;
CREATE TABLE bocnets (
  pkgno varchar(16) default '0',
  notice smallint(6) default NULL,
  area varchar(8) NOT NULL default '',
  codeline smallint(6) default '1',
  exchno varchar(16) NOT NULL default '',
  merge int(4) default '0',
  compare int(4) default '0',
  PRIMARY KEY  (exchno,area)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS clerks;
CREATE TABLE clerks (
  clkno varchar(12) NOT NULL,
  name varchar(20) default '',
  password varchar(16) default NULL,
  sex smallint(6) default '0',
  personalid varchar(20) default '',
  authority int(11) default '0',
  state smallint(6) default '0',
  area varchar(8) default '',
  loginstate smallint(6) default '0',
  logintime timestamp,
  PRIMARY KEY  (clkno)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
insert into clerks(clkno,name,password,sex,personalid,authority, state) values('99999','99999','',0,'',65520,1);

DROP TABLE IF EXISTS config;
CREATE TABLE config (
  name varchar(20) NOT NULL,
  value varchar(20) default '',
  PRIMARY KEY  (name),
  KEY config_name (name)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS dists;
CREATE TABLE dists (
  sid int(11) NOT NULL auto_increment,
  fname varchar(80) default '',
  bname varchar(80) default '',
  ip varchar(24) default '',
  date varchar(8) default '',
  exchno varchar(16) default '',
  pkgno varchar(16) default '',
  session smallint(6) default '0',
  vchno varchar(30) default '000000',
  accno varchar(32) default '',
  vchtype varchar(4) default '00',
  amount varchar(16) default '',
  imageid varchar(80) default '',
  timestamp datetime default NULL,
  scantime datetime default NULL,
  clkno varchar(12) default '',
  times smallint(6) default '0',
  area varchar(8) default '',
  isenv smallint(2) default '0',
  mergestate smallint(2) default '0',
  state smallint(2) default '0',
  mergeid int(11) default -1,
  cdcode smallint(6) default '0',
  modified int(11) default '0',
  PRIMARY KEY  (sid)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS multis;
CREATE TABLE multis (
  sid int(11) NOT NULL auto_increment,
  fname varchar(80) default '',
  bname varchar(80) default '',
  rotate smallint(6) default '0',
  ip varchar(24) default '',
  dpi smallint(6) default '200',
  date varchar(8) default '',
  session smallint(6) default '0',
  exchno varchar(16) default '',
  pkgno varchar(16) default '',
  vchno varchar(30) default '000000',
  accno varchar(32) default '',
  vchtype varchar(4) default '',
  amount varchar(16) default '',
  multiflag smallint(6) default '0',
  state smallint(6) default '0',
  mergestate smallint(6) default '0',
  mergeid int(11) default '0',
  model smallint(6) default '-1',
  mainid int(11) default '0',
  envid int(11) default '0',
  mainflag smallint(6) default '0',
  checkstate smallint(6) default '0',
  prestate smallint(6) default '0',
  sendstate smallint(6) default '0',
  imageid varchar(80) default '',
  timestamp datetime default NULL,
  substate smallint(6) default '0',
  scantime datetime default NULL,
  clkno varchar(12) default '',
  times smallint(6) default '0',
  area varchar(8) default '',
  senddesc varchar(128) default '',
  sendcode smallint(6) default 0,
  modeflag smallint(2) default 0,
  PRIMARY KEY  (sid),
  KEY multis_state (state),
  KEY multis_sendstate (sendstate),
  KEY multis_exchno (exchno,fname,date,session)
) ENGINE=InnoDB AUTO_INCREMENT=1411 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS pbcdata;
CREATE TABLE pbcdata (
  sid int(11) NOT NULL auto_increment,
  vchno varchar(30) default '000000',
  accno varchar(32) default '',
  vchtype varchar(4) default '',
  amount decimal(14,2) default '0.00',
  exchno varchar(16) default '',
  vsexno varchar(16) default '',
  date varchar(10) default '',
  session smallint(6) default '0',
  cdcode varchar(4) default 'C',
  timestamp datetime default NULL,
  mergestate smallint(6) default '0',
  mergeid int(11) default '0',
  prestate smallint(6) default '0',
  batno varchar(4) default '',
  clkno varchar(12) default '',
  fname varchar(80) default '',
  ip varchar(24) default '',
  envelop smallint(6) default '0',
  area varchar(8) default '',
  PRIMARY KEY  (sid),
  KEY pbcdata_exchno (exchno,fname,date,session)
) ENGINE=InnoDB AUTO_INCREMENT=2044 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS pocket;
CREATE TABLE pocket (
  sid int(11) NOT NULL auto_increment,
  pockno int(4) default 0,
  vchtype varchar(4) default '',
  pocdesc varchar(256) default '',
  distno varchar(24) default '',
  PRIMARY KEY  (sid),
  unique key(distno,vchtype),
  KEY pocket_sid(sid)	
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS vchtypes;
CREATE TABLE vchtypes (
  vchtype varchar(4) NOT NULL default '',
  banktype varchar(4) default '',
  model smallint(6) default '0',
  expcn varchar(40) default '',
  cdcode smallint(6) default '0',
  area varchar(8) NOT NULL default '',
  PRIMARY KEY  (vchtype,area),
  KEY vchtypes_vchtype (vchtype)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


DROP TABLE IF EXISTS vouchers;
CREATE TABLE vouchers (
  sid int(11) NOT NULL auto_increment,
  fname varchar(80) default '',
  bname varchar(80) default '',
  rotate smallint(6) default '0',
  ip varchar(24) default '',
  dpi smallint(6) default '200',
  date varchar(8) default '',
  exchno varchar(16) default '',
  pkgno varchar(16) default '',
  session smallint(6) default '0',
  state smallint(6) default '0',
  vchno varchar(30) default '000000',
  accno varchar(32) default '',
  vchtype varchar(4) default '',
  amount varchar(16) default '',
  model smallint(6) default '-1',
  mergestate smallint(6) default '0',
  mergeid int(11) default '0',
  sendstate smallint(6) default '0',
  imageid varchar(80) default '',
  timestamp datetime default NULL,
  submitstate smallint(6) default '0',
  scantime datetime default NULL,
  prestate smallint(6) default '0',
  clkno varchar(12) default '',
  times smallint(6) default '0',
  area varchar(8) default '',
  senddesc varchar(128) default '',
  sendcode smallint(6) default 0,
  modeflag smallint(2) default 0,
  PRIMARY KEY  (sid),
  KEY vouchers_state (state),
  KEY vouchers_sendstate (sendstate),
  KEY vouchers_exchno (exchno,fname,date,session)
) ENGINE=InnoDB AUTO_INCREMENT=1509 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS vips;
CREATE TABLE vips(
  sid int(11) NOT NULL auto_increment,
  accno varchar(32) NOT NULL default '',
  accname varchar(80) default '',
  exchno varchar(16) default '',
  expcn varchar(40) default '',
  PRIMARY KEY  (sid),
  unique key(accno),
  KEY vips_accno (accno)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS authinput;
CREATE TABLE authinput(
  sid int(11) NOT NULL auto_increment,
  fname varchar(80) default '',
  bname varchar(80) default '',
  rotate smallint(6) default '0',
  ip varchar(24) default '',
  dpi smallint(6) default '200',
  date varchar(8) default '',
  session smallint(6) default '0',
  exchno varchar(16) default '',
  pkgno varchar(16) default '',
  vchno varchar(12) default '000000',
  accno varchar(32) default '',
  vchtype varchar(4) default '',
  amount varchar(16) default '',
  multiflag smallint(6) default '0',
  state smallint(6) default '0',
  mergestate smallint(6) default '0',
  mergeid int(11) default '0',
  model smallint(6) default '-1',
  mainid int(11) default '0',
  envid int(11) default '0',
  mainflag smallint(6) default '0',
  checkstate smallint(6) default '0',
  prestate smallint(6) default '0',
  sendstate smallint(6) default '0',
  imageid varchar(80) default '',
  timestamp datetime default NULL,
  substate smallint(6) default '0',
  scantime datetime default NULL,
  clkno varchar(12) default '',
  warclkno varchar(12) default '',
  times smallint(6) default '0',
  area varchar(8) default '',
  senddesc varchar(128) default '',
  sendcode smallint(6) default 0,
  PRIMARY KEY  (sid),
  KEY authinput_state (state),
  KEY authinput_sendstate (sendstate),
  KEY authinput_exchno (exchno,fname,date,session)
) ENGINE=InnoDB AUTO_INCREMENT=1411 DEFAULT CHARSET=latin1;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
