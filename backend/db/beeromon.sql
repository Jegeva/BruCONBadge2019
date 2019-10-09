select 'c beeromons' ;

create table beeromon_element (
       id int,
       ename varchar(50),
       primary key (id)
);

create table beeromon_type (
       id int,
       bname varchar (256),
       pic  varchar (256),
       element int,
       primary key (id),
       foreign key (element) references beeromon_element (id)
);

create table beeromon_power_type (
       id int,
       typename varchar(256),
       primary key (id)
);

create table beeromon_power (
       id int,
       name varchar(256),
       ptype int,
       primary key (id),
       foreign key (ptype) references beeromon_power_type(id)
);
create table beeromon_power_by_type (
       type_id int,
       power_id int,
       primary key (type_id,power_id),
       foreign key (type_id) references beeromon_type(id),
       foreign key (power_id) references beeromon_power(id)
);

create table beeromon_stat (
       type_id int,
       abv     int,  -- str
       bubbles int,  -- def
       ml      int,   -- hp
       primary key (type_id),
       foreign key (type_id) references beeromon_type(id)
);

create table beeromons (
       id int,
       type_id int,
       xp int,
       badge_id varchar(17),
       name varchar(6),
       primary key (id),
       foreign key (type_id)    references beeromon_type(id),
       foreign key (badge_id)   references badges(mac)
);

create table fights (
       id int not null auto_increment,
       op_a int not null,
       op_b int not null,
       ftime datetime not null,
       winner int not null,
       primary key (id),
       unique(op_a,op_b,ftime)
);

create table beeromon_moves(
       id int,
       name varchar(255),
       primary key (id)
);

create table fight_history (
       id int not null,
       fid int,
       op int,
       move int,
       randres1 int,
       randres2 int,
       primary key (id)
);


source beeromondata.sql
