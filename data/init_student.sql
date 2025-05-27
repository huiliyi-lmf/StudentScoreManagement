create table admin
(
    id       INTEGER not null
        primary key autoincrement,
    userName TEXT    not null,
    userPwd  TEXT    not null
);

create table major
(
    id        INTEGER not null
        primary key,
    majorName TEXT    not null
);

create table class
(
    id        INTEGER not null
        primary key autoincrement,
    majorId   INTEGER not null
        constraint fk_major
            references major
            on update restrict on delete restrict,
    className TEXT    not null
);

create table student
(
    id       TEXT    not null
        primary key,
    stuName  TEXT    not null,
    classID  INTEGER not null
        constraint fk_classId
            references class
            on update restrict on delete restrict,
    majorID  INTEGER not null
        constraint fk_majorId
            references major
            on update restrict on delete restrict,
    age      INTEGER not null,
    sex      TEXT    not null,
    phoneNum TEXT    not null,
    address  TEXT    not null
);

create table subject
(
    id      INTEGER not null
        primary key autoincrement,
    majorId INTEGER not null
        constraint fk_majorId
            references major
            on update restrict on delete restrict,
    subName TEXT    not null
);

create table score
(
    id    INTEGER not null
        primary key autoincrement,
    stuId INTEGER not null
        constraint fk_stuId
            references student
            on update restrict on delete restrict,
    subId INTEGER not null
        constraint fk_subId
            references subject
            on update restrict on delete restrict,
    score REAL    not null
);


