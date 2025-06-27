#1.) Search function:
if item.ITEM_TYPE_WEAPON == itemType:
#2.) Add before !!!:
		if app.TOURNAMENT_PVP_SYSTEM:
			if itemVnum in (84201, 84202):

				self.DESCRIPTION_VNUMS = {
					84201: localeInfo.TOURNAMENT_ITEM_BOX_REWARD,
					84202: localeInfo.TOURNAMENT_ITEM_HEART,
				}

				self.AppendDescription(self.DESCRIPTION_VNUMS[itemVnum], None, self.HIGH_PRICE_COLOR)