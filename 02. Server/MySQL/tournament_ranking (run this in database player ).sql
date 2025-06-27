
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `tournament_ranking`
-- ----------------------------
DROP TABLE IF EXISTS `tournament_ranking`;
CREATE TABLE `tournament_ranking` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pid` int(11) NOT NULL,
  `name` varchar(12) NOT NULL,
  `total_points` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;