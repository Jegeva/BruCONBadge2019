drop database if exists bruconbadges2019;

create user if not exists 'jg'@'localhost' IDENTIFIED BY 'cj9q6a2EpMF1zTXJ7c4X';
create user if not exists 'bruconbadges_php'@'localhost' IDENTIFIED by '9T8sxEiMCSq3lFzJWIy2';

create database if not exists bruconbadges2019;
use bruconbadges2019;


select 'd quizz' ;

drop table if exists badge_quizz;
drop table if exists quizz_q;
drop table if exists quizz_r;

select 'd beeromon' ;
drop table if exists beeromon_power;
drop table if exists beeromon_power_type;
drop table if exists beeromon_stat;
drop table if exists beeromon_type;
drop table if exists beeromon_element;
drop table if exists beeromon_moves;
drop table if exists fights;
drop table if exists fight_history;


select 'd badges' ;
drop table if exists badges;

select 'c badges' ;
create table badges (
mac varchar(17),
username varchar(256),
isgame boolean,
time_enroll datetime not null,

            primary key (mac)
);



source beeromon.sql

GRANT ALL PRIVILEGES ON bruconbadges2019.* TO 'jg'@'localhost' ;
GRANT SELECT on bruconbadges2019.* TO 'bruconbadges_php'@'localhost' ;
GRANT INSERT on bruconbadges2019.badges TO 'bruconbadges_php'@'localhost';
GRANT INSERT on bruconbadges2019.fights TO 'bruconbadges_php'@'localhost';
GRANT INSERT on bruconbadges2019.fight_history TO 'bruconbadges_php'@'localhost';
GRANT UPDATE (username) on bruconbadges2019.badges TO 'bruconbadges_php'@'localhost';

