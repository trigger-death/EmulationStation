#pragma once

#include "views/gamelist/IGameListView.h"

#include "components/TextComponent.h"
#include "components/ImageComponent.h"
#include "GameDataGame.h"
#include "GameDataFolder.h"

class ISimpleGameListView : public IGameListView
{
public:
	ISimpleGameListView(Window* window, GameDataList* root);
	virtual ~ISimpleGameListView() {}

	// Called when a new file is added, a file is removed, a file's metadata changes, or a file's children are sorted.
	virtual void onFileChanged(GameDataItem* file, FileChangeType change);
	
	// Called whenever the theme changes.
	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	virtual GameDataItem* getCursor() = 0;
	virtual void setCursor(GameDataItem*) = 0;

	virtual bool input(InputConfig* config, Input input) override;

protected:
	virtual void populateList(const std::vector<GameDataItem*>& files) = 0;
	virtual void launch(GameDataGame* game) = 0;

	TextComponent mHeaderText;
	ImageComponent mHeaderImage;
	ImageComponent mBackground;
	
	ThemeExtras mThemeExtras;

	std::stack<GameDataItem*> mCursorStack;
};
