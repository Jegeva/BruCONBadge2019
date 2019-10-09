select 'c quizz' ;

create table quizz_r (
id int,
r varchar(256),
            primary key (id)
);

create table quizz_q (
id int,
q varchar(256),
r0 int,
r1 int,
r2 int,
 primary key (id),
 foreign key (r0) references quizz_r (id),
 foreign key (r1) references quizz_r (id),
 foreign key (r2) references quizz_r (id)
);

create table badge_quizz (
       id int,
       q_id int,
       badge_id varchar(17),
       time_answer datetime not null,
       answer_right int,
       primary key (id),
       foreign key (q_id) references quizz_q (id),
       foreign key (badge_id) references badges (mac)
);
