DROP DATABASE IF EXISTS `slipad`;
CREATE DATABASE `slipad`;
USE `slipad`;

DROP TABLE IF EXISTS `operator`;
CREATE TABLE `operator`(
	`id` INTEGER AUTO_INCREMENT,
	`name` CHAR(50) NOT NULL,
	`password` CHAR(50) NOT NULL,
	PRIMARY KEY(`id`)
);

DROP TABLE IF EXISTS `region`;
CREATE TABLE `region`(
	`post_code` CHAR(8) NOT NULL,
	`operator_id` INTEGER NOT NULL AUTO_INCREMENT,
	`parish` CHAR(50) NOT NULL,
	`county` CHAR(50) NOT NULL,
	`district` CHAR(50) NOT NULL,
	PRIMARY KEY(`post_code`),
	FOREIGN KEY(`operator_id`) REFERENCES `operator`(`id`)
);

DROP TABLE IF EXISTS `location`;
CREATE TABLE `location`(
	-- `gps` CHAR(50) NOT NULL,
	`latitude` DECIMAL(8,6),
	`longitude` DECIMAL(9,6),
	`post_code` CHAR(8) NOT NULL,
	`street_name` CHAR(50) NOT NULL,
	PRIMARY KEY(`latitude`,`longitude`),
	FOREIGN KEY(`post_code`) REFERENCES `region`(`post_code`)
);

DROP TABLE IF EXISTS `lamppost`;
CREATE TABLE `lamppost`(
	`id` INTEGER NOT NULL AUTO_INCREMENT,
	-- `gps` CHAR(50) NOT NULL,
	`latitude` DECIMAL(8,6),
	`longitude` DECIMAL(9,6),
	`status` ENUM('FAIL', 'OFF', 'ON', 'MIN'),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`latitude`,`longitude`) REFERENCES location(`latitude`,`longitude`)
);

DROP TABLE IF EXISTS `parking_space`;
CREATE TABLE `parking_space`(
	`id` INTEGER NOT NULL AUTO_INCREMENT,
	`latitude` DECIMAL(8,6),
	`longitude` DECIMAL(9,6),
	-- `type` CHAR,
	`status` ENUM('AVAILABLE', 'OCCUPIED'),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`latitude`,`longitude`) REFERENCES `location`(`latitude`,`longitude`)
);
