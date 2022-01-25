DROP DATABASE IF EXISTS slipad;
CREATE DATABASE slipad;
USE slipad;

CREATE TABLE lamppost(
	id INTEGER NOT NULL AUTO_INCREMENT,
	gps DOUBLE[2],
	status CHAR,
	PRIMARY KEY(id),
	FOREIGN KEY(gps) REFERENCES location(gps)
);

CREATE TABLE location(
	gps DOUBLE[2],
	post_code INTEGER NOT NULL,
	street_name CHAR[50] NOT NULL,
	PRIMARY KEY(gps),
	FOREIGN KEY(post_code) REFERENCES region(post_code)
);

CREATE TABLE operator(
	id INTEGER NOT NULL AUTO_INCREMENT,
	name CHAR[50] NOT NULL,
	password CHAR[50] NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE region(
	post_code INTEGER NOT NULL,
	operator_id INTEGER NOT NULL AUTO_INCREMENT,
	parish CHAR[50] NOT NULL,
	county CHAR[50] NOT NULL,
	district CHAR[50] NOT NULL,
	PRIMARY KEY(post_code),
	FOREIGN KEY(operator_id) REFERENCES operator(id)
);

CREATE TABLE parking_space(
	id INTEGER NOT NULL AUTO_INCREMENT,
	gps DOUBLE[2],
	type CHAR,
	status CHAR,
	PRIMARY KEY(id),
	FOREIGN KEY(gps) REFERENCES location(gps)
);
