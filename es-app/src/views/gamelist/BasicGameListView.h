#pragma once

#include "views/gamelist/ISimpleGameListView.h"
#include "components/TextListComponent.h"

class BasicGameListView : public ISimpleGameListView
{
public:
	BasicGameListView(Window* window, GameDataList* root);

	// Called when a FileData* is added, has its metadata changed, or is removed
	virtual void onFileChanged(GameDataItem* file, FileChangeType change);

	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	virtual GameDataItem* getCursor() override;
	virtual void setCursor(GameDataItem* file) override;

	virtual const char* getName() const override { return "basic"; }

	virtual std::vector<HelpPrompt> getHelpPrompts() override;

protected:
	virtual void populateList(const std::vector<GameDataItem*>& files) override;
	virtual void launch(GameDataGame* game) override;
	virtual void remove(GameDataItem* game) override;

	TextListComponent<GameDataItem*> mList;
};
