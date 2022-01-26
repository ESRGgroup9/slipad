DROP DATABASE IF EXISTS `slipad`;
CREATE DATABASE `slipad`;
USE `slipad`;

DROP TABLE IF EXISTS `operator`;
CREATE TABLE `operator`(
	`id` INTEGER NOT NULL AUTO_INCREMENT,
	`name` CHAR(50) NOT NULL,
	`password` CHAR(50) NOT NULL,
	PRIMARY KEY(`id`)
);

DROP TABLE IF EXISTS `region`;
CREATE TABLE `region`(
	`post_code` CHAR(8) NOT NULL,
	`operator_id` INTEGER NOT NULL,
	`parish` CHAR(50) NOT NULL,
	`county` CHAR(50) NOT NULL,
	`district` CHAR(50) NOT NULL,
	PRIMARY KEY(`post_code`),
	FOREIGN KEY(`operator_id`) REFERENCES `operator`(`id`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `location`;
CREATE TABLE `location`(
	`id` INTEGER NOT NULL,
	`latitude` DECIMAL(8,6) NOT NULL,
	`longitude` DECIMAL(9,6) NOT NULL,
	`post_code` CHAR(8) NOT NULL,
	`street_name` CHAR(50) NOT NULL,
	PRIMARY KEY(`id`),
	FOREIGN KEY(`post_code`) REFERENCES `region`(`post_code`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `lamppost`;
CREATE TABLE `lamppost`(
	`id` INTEGER NOT NULL AUTO_INCREMENT,
	`address` INTEGER NOT NULL,
	`status` ENUM('FAIL', 'OFF', 'ON', 'MIN'),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`id`) REFERENCES `location`(`id`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `parking_space`;
CREATE TABLE `parking_space`(
	`id` INTEGER NOT NULL,
	`num_vacants` INTEGER DEFAULT(0),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`id`) REFERENCES `lamppost`(`id`)
		ON UPDATE CASCADE
);
