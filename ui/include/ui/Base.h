#pragma once

#include <optional>
#include <variant>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include <mem/ReferenceManager.h>
#include <misc/Rectangle.h>

#include "ControlState.h"

#include "Enums.h"

class UIOBase;
struct RenderInfo;
class GameState;

class UIOBase;
struct PlayerState;

namespace ui
{
	namespace BIND
	{
		enum RESULT
		{
			CONTINUE = 1 << 0,
			CLOSE = 1 << 1,
			STOP = 1 << 2,
			FOCUS = 1 << 3,
			CONSUME = 1 << 4,
			HIDE = 1 << 5,
		};
	}

	typedef int32_t CallBackBindResult;

	typedef PlayerState PlayerState;

	typedef std::function<CallBackBindResult(PlayerState& PlayerState, UIOBase* self)> CallBack;

	class UIState;

	class UIOBase
	{
	protected:
		bool active = false;

		using Bind = std::pair<BindControl, CallBack>;

		std::vector<Bind> onHoverBinds;
		std::vector<Bind> activeBinds;
		std::vector<Bind> focussedBinds;
		std::vector<Bind> globalBinds;
		std::vector<Bind> gameWorldBinds;

	public:
		Handle selfHandle;

		Handle getSelfHandle();

		ScreenRectangle screenRectangle;

		virtual void addElement(UniqueReference<UIOBase, UIOBase> element) = 0;
		virtual void translate(glm::vec2 p) = 0;
		virtual void setScreenPixels(glm::ivec2 px) = 0;

		bool contains(glm::vec2 p) const;

		void activate();
		void deactivate();

		void moveTopLeftTo(glm::vec2 p);

		void addGlobalBind(BindControl bindControl, CallBack callBack);
		void addFocussedBind(BindControl bindControl, CallBack callBack);
		void addOnHoverBind(BindControl bindControl, CallBack callBack);
		void addActiveBind(BindControl bindControl, CallBack callBack);
		void addGameWorldBind(BindControl bindControl, CallBack callBack);

		void addGlobalBinds(std::vector<BindControl> const& bindControls, CallBack callBack);
		void addFocussedBinds(std::vector<BindControl> const& bindControls, CallBack callBack);
		void addOnHoverBinds(std::vector<BindControl> const& bindControls, CallBack callBack);
		void addActiveBinds(std::vector<BindControl> const& bindControls, CallBack callBack);
		void addGameWorldBinds(std::vector<BindControl> const& bindControls, CallBack callBack);

		virtual CallBackBindResult runGlobalBinds(PlayerState& playerState);
		virtual CallBackBindResult runFocussedBinds(PlayerState& playerState);
		virtual CallBackBindResult runOnHoverBinds(PlayerState& playerState);
		virtual CallBackBindResult runActiveBinds(PlayerState& playerState);
		virtual CallBackBindResult runGameWorldBinds(PlayerState& playerState);

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

		virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) = 0;

		ScreenRectangle const& getScreenRectangle() const;

		virtual TYPE getUIOType();

		virtual ~UIOBase() = default;
	};

	class UIOBaseMulti : public UIOBase
	{
	protected:
		std::vector<UniqueReference<UIOBase, UIOBase>> elements;

	public:
		void clear();

		virtual void addElement(UniqueReference<UIOBase, UIOBase> element) override;

		virtual void translate(glm::vec2 p) override;
		virtual void setScreenPixels(glm::ivec2 px) override;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual CallBackBindResult runGlobalBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runFocussedBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runOnHoverBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runActiveBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runGameWorldBinds(PlayerState& playerState) override;

		virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

		UIOBaseMulti() = default;
		virtual ~UIOBaseMulti() = default;
	};

	class UIOBaseSingle : public UIOBase
	{
	protected:
		UniqueReference<UIOBase, UIOBase> main;

	public:
		virtual void addElement(UniqueReference<UIOBase, UIOBase> element) override;

		virtual void translate(glm::vec2 p) override;
		virtual void setScreenPixels(glm::ivec2 px) override;

		virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

		virtual CallBackBindResult runGlobalBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runFocussedBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runOnHoverBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runActiveBinds(PlayerState& playerState) override;
		virtual CallBackBindResult runGameWorldBinds(PlayerState& playerState) override;

		virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

		UIOBaseSingle() = default;
		virtual ~UIOBaseSingle() = default;
	};

	class UIOBaseEnd : public UIOBase
	{
	private:
		virtual void addElement(UniqueReference<UIOBase, UIOBase> element) override;

	public:
		virtual void translate(glm::vec2 p) override;
		virtual void setScreenPixels(glm::ivec2 px) override;

		UIOBaseEnd() = default;
		virtual ~UIOBaseEnd() = default;
	};
}
