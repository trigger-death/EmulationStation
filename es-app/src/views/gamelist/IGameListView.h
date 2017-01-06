#pragma once

#include "FileData.h"
#include "Renderer.h"
#include "GameDataList.h"
#include "GameDataItem.h"

class Window;
class GuiComponent;
class FileData;
class ThemeData;

// This is an interface that defines the minimum for a GameListView.
class IGameListView : public GuiComponent
{
public:
	IGameListView(Window* window, GameDataList* root) : GuiComponent(window), mRoot(root)
		{ setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight()); }

	virtual ~IGameListView() {}

	// Called when a new file is added, a file is removed, a file's metadata changes, or a file's children are sorted.
	virtual void onFileChanged(GameDataItem* file, FileChangeType change) = 0;
	
	// Called whenever the theme changes.
	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme) = 0;

	void setTheme(const std::shared_ptr<ThemeData>& theme);
	inline const std::shared_ptr<ThemeData>& getTheme() const { return mTheme; }

	virtual GameDataItem* getCursor() = 0;
	virtual void setCursor(GameDataItem*) = 0;

	virtual bool input(InputConfig* config, Input input) override;
	virtual void remove(GameDataItem* game) = 0;

	virtual const char* getName() const = 0;

	virtual HelpStyle getHelpStyle() override;
protected:
	GameDataList* mRoot;
	std::shared_ptr<ThemeData> mTheme;
};
