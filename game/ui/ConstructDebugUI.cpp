#include "ui/ConstructDebugUI.h"

#include <mem/ReferenceManager.h>

#include <ui/Constructer.h>
#include <ui/List.h>
#include <ui/Empty.h>
#include <ui/Button.h>
#include <ui/TextDisplay.h>
#include <ui/SizeType.h>
#include <ui/AnchoredProxy.h>
#include <ui/binds/Binds.h>
#include <ui/ControlState.h>
#include <ui/State.h>

#include <render/Colors.h>
#include <render/BufferWrappers.h>

#include <mem/Locator.h>

#include <misc/Timer.h>
#include <misc/Option.h>
#include <misc/Log.h>
#include <misc/StringHelpers.h>

#include "player/PlayerInfo.h"

namespace game
{
	WeakReference<ui::Base, ui::List> constructDebugUI() {
		auto mainList = ui::startList(ui::DIR::DOWN);

		// ---------
		// Tick info
		// ---------

		ui::constrainHeight({ ui::SIZETYPE::FH, 16.3f });
		ui::background(COLORS::UI::BACKGROUND);
		auto tickInfo = ui::textDisplayMulti("TODO: add bind");

		ui::constrainHeight({ ui::SIZETYPE::STATIC_PX, 4 });
		ui::makeEnd<ui::Empty>();

		// ---------------------
		// Save and Load buttons
		// ---------------------

		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		ui::background(COLORS::UI::BACKGROUND);
		auto saveName = ui::textEditSingle("test.save");

		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		ui::startList(ui::DIR::RIGHT);

		ui::constrainWidth({ ui::SIZETYPE::RELATIVE_WIDTH, 0.5f });
		auto saveButton = ui::textButton("save");

		saveButton.get()->setOnPress([saveName = saveName.get()](PlayerInfo& playerInfo, ui::Base* self) {
			auto const name = misc::trim(saveName->text.getLines()[0]);
			playerInfo.uiState.saveGame = name;

			return ui::BIND::RESULT::CONTINUE;
			}
		);

		auto loadButton = ui::textButton("load");

		loadButton.get()->setOnPress([saveName = saveName.get()](PlayerInfo& playerInfo, ui::Base* self) {
			auto const name = misc::trim(saveName->text.getLines()[0]);
			playerInfo.uiState.loadGame = name;

			return ui::BIND::RESULT::CONTINUE;
			}
		);

		ui::endList();

		ui::constrainHeight({ ui::SIZETYPE::STATIC_PX, 4 });
		ui::makeEnd<ui::Empty>();

		// ---------------------------------------------------
		// Toggles for Debug Render and Seperate Render Thread
		// ---------------------------------------------------

		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		auto debugButton = ui::textButton("Debug Render");

		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		auto renderThread = ui::textButton("Toggle Seperate Render Thread");

		// -----------------------------
		// Button for opening Builder UI
		// -----------------------------

		ui::constrainHeight({ ui::SIZETYPE::ABSOLUTE_HEIGHT, 0.0f });
		auto proxy = ui::makeEnd<ui::AnchoredProxy>();
		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		auto builderTest = ui::textButton("Builder");

		// ----------------------------------
		// Button for opening Item Spawner UI
		// ----------------------------------

		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		auto spawnItem = ui::textButton("Spawn Item");

		// ------------------------------------------------
		// Button for opening different information windows
		// ------------------------------------------------

		//ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		//auto info = ui::textButton("Info");


		ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
		ui::menu("Info", std::nullopt, []() {
			ui::startList(ui::DIR::DOWN);
			ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
			ui::menu("Info", { { ui::SIZETYPE::FH, 20.0f } }, []() {
				//ui::constrainHeight({ ui::SIZETYPE::FH, 20.0f });
				ui::startList(ui::DIR::DOWN);

				ui::constrainHeight({ ui::SIZETYPE::FH, 10.0f });
				ui::textDisplayMulti(render::bwo::Program::listAll());

				//ui::constrainHeight({ ui::SIZETYPE::FH, 1.2f });
				//ui::textButton("test");
				ui::endList();
				});
			ui::endList();
			});


		ui::constrainHeight({ ui::SIZETYPE::STATIC_PX, 4 });
		ui::makeEnd<ui::Empty>();

		// ----------
		// Log output
		// ----------

		ui::background(COLORS::UI::BACKGROUND);
		auto logOutput = ui::textDisplayMulti("");
		ui::BASE::activatable(logOutput.get());

		ui::endList();

		// ---
		// End
		// ---

		// -----
		// Binds
		// -----

		tickInfo.get()->addGlobalBind(
			{ ui::CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(ui::CONTROL::STATE::PRESSED) },
			[](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult
			{
				auto self = static_cast<ui::TextDisplay*>(self_);
				self->setText(Locator<misc::Timer>::ref().print());
				return ui::BIND::RESULT::CONTINUE;
			});

		debugButton.get()->setOnRelease([](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult
			{
				misc::Option<misc::OPTION::GR_DEBUG, bool>::setVal(!misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal());
				auto self = static_cast<ui::Button*>(self_);
				self->setColor(misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
				return ui::BIND::RESULT::CONTINUE;
			});
		debugButton.get()->setColor(misc::Option<misc::OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

		renderThread.get()->setOnRelease([](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult
			{
				misc::Option<misc::OPTION::GR_RENDERTHREAD, bool>::setVal(!misc::Option<misc::OPTION::GR_RENDERTHREAD, bool>::getVal());
				auto self = static_cast<ui::Button*>(self_);
				self->setColor(misc::Option<misc::OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
				return ui::BIND::RESULT::CONTINUE;
			});
		renderThread.get()->setColor(misc::Option<misc::OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

		logOutput.get()->addGlobalBind({ ui::CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(ui::CONTROL::STATE::PRESSED) }, [](PlayerInfo& playerInfo, ui::Base* self_) -> ui::CallBackBindResult
			{
				auto self = static_cast<ui::TextDisplay*>(self_);

				auto& vec = self->text.getLinesMutable();
				if (vec.size() > 100) {
					vec.erase(vec.begin(), vec.begin() + 50);
				}

				auto newLines = Locator<misc::Log>::ref().getLines();
				for (auto& newLine : newLines) {
					self->text.addLine(newLine);
				}
				if (newLines.size() != 0) {
					self->text.moveCursor(glm::ivec2(100000, newLines.size()));
				}
				return ui::BIND::RESULT::CONTINUE;
			});

		return mainList;
	}
}
