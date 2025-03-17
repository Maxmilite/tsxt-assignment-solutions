-- MySQL dump 10.13  Distrib 8.0.32, for Win64 (x86_64)
--
-- Host: localhost    Database: userdata
-- ------------------------------------------------------
-- Server version	8.0.32

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `chat_history`
--

DROP TABLE IF EXISTS `chat_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `chat_history` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `sender` int NOT NULL,
  `message_sent` mediumtext NOT NULL,
  `message_received` mediumtext NOT NULL,
  `category` tinytext,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_history`
--

LOCK TABLES `chat_history` WRITE;
/*!40000 ALTER TABLE `chat_history` DISABLE KEYS */;
INSERT INTO `chat_history` VALUES (1,'2023-03-11 20:54:49',1,'Nice to meet you','Nice to meet you too.','Message'),(2,'2023-03-11 20:56:10',1,'Never put off what you can do today until tomorrow','今日事今日毕！','Consumption'),(3,'2023-03-11 20:59:15',1,'You think you can, you can','你认为你行，你就行。','Default'),(4,'2023-03-11 22:03:28',1,'Believe in yourself','相信你自己！','Education'),(5,'2023-03-11 22:03:31',1,'Nice to meet you','Nice to meet you too.','Entertainment'),(6,'2023-03-11 22:51:13',1,'Nice to meet you','Nice to meet you too.','Default'),(7,'2023-03-11 22:51:15',1,'Believe in yourself','相信你自己！','Default'),(8,'2023-03-11 22:51:17',1,'Keep on going never give up','勇往直前，决不放弃！','Default'),(9,'2023-03-11 22:51:19',1,'Believe in yourself','相信你自己！','Default'),(10,'2023-03-11 22:52:31',1,'Winners do what losers dont want to do','胜利者做失败者不愿意做的事！','Default'),(11,'2023-03-11 22:52:33',1,'你支持中文问答吗','是的，我支持。','Default'),(12,'2023-03-11 22:52:35',1,'你和 ChatGPT 有什么联系','毫无联系，我只是一个能按照语料库回复的机器人罢了。','Consumption'),(13,'2023-03-11 22:52:37',1,'Believe in yourself','相信你自己！','Education'),(14,'2023-03-11 22:52:40',1,'You think you can, you can','你认为你行，你就行。','Default'),(15,'2023-03-11 22:52:46',1,'Jack of all trades and master of none','门门精通，样样稀松。','Default'),(16,'2023-03-11 22:53:24',1,'Believe in yourself','相信你自己！','Education'),(17,'2023-03-11 22:53:27',1,'How are you','Im fine, thank you.','Default'),(18,'2023-03-11 22:53:30',1,'你和 ChatGPT 有什么联系','毫无联系，我只是一个能按照语料库回复的机器人罢了。','Default'),(19,'2023-03-11 22:53:31',1,'Keep on going never give up','勇往直前，决不放弃！','Default'),(20,'2023-03-11 22:54:48',1,'你和 ChatGPT 有什么联系','毫无联系，我只是一个能按照语料库回复的机器人罢了。','Default'),(21,'2023-03-12 00:17:11',1,'You think you can, you can','你认为你行，你就行。','Default'),(22,'2023-03-12 15:25:31',1,'Never put off what you can do today until tomorrow','今日事今日毕！','Default'),(23,'2023-03-12 15:28:44',1,'Never put off what you can do today until tomorrow','今日事今日毕！','Education'),(24,'2023-03-12 21:43:06',1,'你支持中文问答吗','是的，我支持。','Consumption'),(25,'2023-03-12 21:43:10',1,'Never put off what you can do today until tomorrow','今日事今日毕！','Default'),(26,'2023-03-12 21:43:13',1,'How are you','Im fine, thank you.','Entertainment'),(27,'2023-03-12 22:42:27',1,'你支持中文问答吗','是的，我支持。','Default'),(28,'2023-03-12 22:43:34',1,'Keep on going never give up','勇往直前，决不放弃！','Default');
/*!40000 ALTER TABLE `chat_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_pass`
--

DROP TABLE IF EXISTS `user_pass`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_pass` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `username` mediumtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `password` mediumtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_pass`
--

LOCK TABLES `user_pass` WRITE;
/*!40000 ALTER TABLE `user_pass` DISABLE KEYS */;
INSERT INTO `user_pass` VALUES (1,'Mike','pbkdf2:sha256:260000$MM9WoW2EVpK8xtlS$8bd16c0f16c17162e5ec3f5c518583bc7515244bb0632d868ff58aabb190553b'),(3,'David','pbkdf2:sha256:260000$G81ZCGocDxccB13n$807bd02bd5f74233af018e52879f90a6aff4d4b52181e5b07ebbaca2407c1662');
/*!40000 ALTER TABLE `user_pass` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-03-12 22:48:16
-- MySQL dump 10.13  Distrib 8.0.32, for Win64 (x86_64)
--
-- Host: localhost    Database: messagetemplate
-- ------------------------------------------------------
-- Server version	8.0.32

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `message_table`
--

DROP TABLE IF EXISTS `message_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `message_table` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `question` mediumtext NOT NULL,
  `answer` mediumtext NOT NULL,
  `popularity` int(10) unsigned zerofill NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `message_table`
--

LOCK TABLES `message_table` WRITE;
/*!40000 ALTER TABLE `message_table` DISABLE KEYS */;
INSERT INTO `message_table` VALUES (1,'Nice to meet you','Nice to meet you too.',0000000026),(2,'How are you','Im fine, thank you.',0000000048),(3,'你支持中文问答吗','是的，我支持。',0000000032),(4,'你和 ChatGPT 有什么联系','毫无联系，我只是一个能按照语料库回复的机器人罢了。',0000000054),(12,'Keep on going never give up','勇往直前，决不放弃！',0000000021),(13,'Never put off what you can do today until tomorrow','今日事今日毕！',0000000038),(14,'Believe in yourself','相信你自己！',0000000043),(15,'You think you can, you can','你认为你行，你就行。',0000000008),(16,'I can because i think i can','我行，因为我相信我行！',0000000005),(17,'Action speak louder than words','行动胜于言语！',0000000008),(18,'Never say die','永不气馁！',0000000002),(19,'Winners do what losers dont want to do','胜利者做失败者不愿意做的事！',0000000009),(20,'Jack of all trades and master of none','门门精通，样样稀松。',0000000011);
/*!40000 ALTER TABLE `message_table` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-03-12 22:48:16
