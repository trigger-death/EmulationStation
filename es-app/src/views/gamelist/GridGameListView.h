#pragma once

#include "views/gamelist/ISimpleGameListView.h"
#include "components/ImageGridComponent.h"
#include "components/ImageComponent.h"
#include <stack>

class GridGameListView : public ISimpleGameListView
{
public:
	GridGameListView(Window* window, GameDataList* root);

	//virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) override;

	virtual GameDataItem* getCursor() override;
	virtual void setCursor(GameDataItem*) override;

	virtual bool input(InputConfig* config, Input input) override;

	virtual const char* getName() const override { return "grid"; }

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

protected:
	virtual void populateList(const std::vector<GameDataItem*>& files) override;
	virtual void launch(GameDataGame* game) override;

	ImageGridComponent<GameDataItem*> mGrid;
};
