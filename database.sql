create table admin
(
    id       INTEGER not null
        primary key autoincrement,
    userName TEXT    not null,
    userPwd  TEXT    not null
);

create table class
(
    id        INTEGER not null
        primary key autoincrement,
    majorId   INTEGER not null,
    className TEXT    not null
);

create table major
(
    id        INTEGER not null
        primary key,
    majorName TEXT    not null
);

create table score
(
    id    INTEGER not null
        primary key autoincrement,
    stuId INTEGER not null,
    subId INTEGER not null,
    score REAL    not null
);


create table student
(
    id       TEXT    not null
        primary key,
    stuName  TEXT    not null,
    classID  INTEGER not null,
    majorID  INTEGER not null,
    age      INTEGER not null,
    sex      TEXT    not null,
    phoneNum TEXT    not null,
    address  TEXT    not null
);

create table subject
(
    id      INTEGER not null
        primary key autoincrement,
    majorId INTEGER not null,
    subName TEXT    not null
);


