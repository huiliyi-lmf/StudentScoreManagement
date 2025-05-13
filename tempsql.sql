/*
 Navicat MySQL Data Transfer

 Source Server         : VLSMB
 Source Server Type    : MySQL
 Source Server Version : 50735
 Source Host           : localhost:3306
 Source Schema         : studentdb

 Target Server Type    : MySQL
 Target Server Version : 50735
 File Encoding         : 65001

 Date: 08/02/2022 23:05:16
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for admin
-- ----------------------------
CREATE TABLE IF NOT EXISTS `admin`  (
  `userName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `userPwd` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`userPwd`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of admin
-- ----------------------------

-- ----------------------------
-- Table structure for class
-- ----------------------------
-- 需要主键id int(11) priKey。MySQL -> SQLite
CREATE TABLE IF NOT EXISTS `class`  (
  `classID` int(11) NOT NULL,
  `gradeID` int(11) NOT NULL,
  `className` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`classID`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of class
-- ----------------------------

-- ----------------------------
-- Table structure for grade
-- ----------------------------
CREATE TABLE IF NOT EXISTS `grade`  (
  `gradeID` int(11) NOT NULL,
  `gradeName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`gradeID`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of grade
-- ----------------------------

-- ----------------------------
-- Table structure for result
-- ----------------------------
CREATE TABLE IF NOT EXISTS `result`  (
  `ID` int(11) NOT NULL,
  `stuID` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `kindID` int(11) NOT NULL,
  `subID` int(11) NOT NULL,
  `result` double NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of result
-- ----------------------------

-- ----------------------------
-- Table structure for student
-- ----------------------------
CREATE TABLE IF NOT EXISTS `student`  (
  `stuID` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `stuName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `classID` int(11) NOT NULL,
  `gradeID` int(11) NOT NULL,
  `age` int(11) NOT NULL,
  `sex` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `phone` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `address` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`stuID`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of student
-- ----------------------------

-- ----------------------------
-- Table structure for subject
-- ----------------------------
CREATE TABLE IF NOT EXISTS `subject`  (
  `subID` int(11) NOT NULL,
  `subName` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`subID`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of subject
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
