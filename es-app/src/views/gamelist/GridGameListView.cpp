#include "views/gamelist/GridGameListView.h"
#include "ThemeData.h"
#include "Window.h"
#include "views/ViewController.h"

GridGameListView::GridGameListView(Window* window, GameDataList* root) : ISimpleGameListView(window, root),
	mGrid(window)
{
	mGrid.setPosition(0, mSize.y() * 0.2f);
	mGrid.setSize(mSize.x(), mSize.y() * 0.8f);
	addChild(&mGrid);

	populateList(root->folder()->items());
}

GameDataItem* GridGameListView::getCursor()
{
	return mGrid.getSelected();
}

void GridGameListView::setCursor(GameDataItem* file)
{
	if(!mGrid.setCursor(file))
	{
		populateList(file->parent()->items());
		mGrid.setCursor(file);
	}
}

bool GridGameListView::input(InputConfig* config, Input input)
{
	if(config->isMappedTo("left", input) || config->isMappedTo("right", input))
		return GuiComponent::input(config, input);

	return ISimpleGameListView::input(config, input);
}

void GridGameListView::populateList(const std::vector<GameDataItem*>& files)
{
	mGrid.clear();
	for(auto it = files.begin(); it != files.end(); it++)
	{
		//mGrid.add((*it)->name(), (*it)->getThumbnailPath(), *it);
	}
}

void GridGameListView::launch(GameDataGame* game)
{
	ViewController::get()->launch(game);
}

std::vector<HelpPrompt> GridGameListView::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("up/down/left/right", "scroll"));
	prompts.push_back(HelpPrompt("a", "launch"));
	prompts.push_back(HelpPrompt("b", "back"));
	return prompts;
}
