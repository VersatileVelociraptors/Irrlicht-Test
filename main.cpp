#include <iostream>
#include <irrlicht.h>

class EventReceiver : public irr::IEventReceiver {
private:
	bool keyIsDown[irr::KEY_KEY_CODES_COUNT];
	int mouseX, mouseY;

public:
	EventReceiver() {
		for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) {
			keyIsDown[i] = false;
		}
		mouseX = mouseY = 0;
	}

	virtual bool OnEvent(const irr::SEvent& event) {
		if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
			keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		} else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
			// update x and y when mouse moves
			mouseX = event.MouseInput.X;
			mouseY = event.MouseInput.Y;
		}
		return false;
	}

	bool isKeyDown(irr::EKEY_CODE keyCode) const {
		return keyIsDown[keyCode];
	}

	int getMouseX() const {
		return mouseX;
	}

	int getMouseY() const {
		return mouseY;
	}
};

// return color with pseudorandom RGB values
irr::video::SColor randomColor(irr::IRandomizer* randomizer){
	return irr::video::SColor(0, (int) (randomizer->frand() * 255), (int) (randomizer->frand() * 255),
			(int) (randomizer->frand() * 255));
}

int main(int argc, char** argv){
	EventReceiver receiver;
	irr::core::dimension2d<irr::u32> resolution = irr::core::dimension2d<irr::u32>(640 * 1.5, 360 * 1.5);
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_SOFTWARE,
		resolution, 16, false, false, false, &receiver);

	if (!device){
		std::cerr << "No IrrlichtDevice" << std::endl;
		return 1;
	}

	device->setWindowCaption(L"Irrlicht-Test");
	device->setResizable(false);

	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	// image assets
	irr::video::ITexture* coco = driver->getTexture("assets/images/cocoGuy.png");
	irr::video::ITexture* logo = driver->getTexture("assets/images/VersatileVelociraptors.png");

	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().AntiAliasing = irr::video::EAAM_FULL_BASIC;

	int coco_x = 0, coco_y = 0, color_update = 0;
	irr::video::SColor background(0, 255, 255, 255);

	while (device->run()){
		// move coco guy
		if (receiver.isKeyDown(irr::KEY_DOWN) && coco_y + coco->getSize().Height < resolution.Height) {
			coco_y += 3;
		} else if (receiver.isKeyDown(irr::KEY_UP) && coco_y > 0) {
			coco_y -= 3;
		}
		if (receiver.isKeyDown(irr::KEY_RIGHT) && coco_x + coco->getSize().Width < resolution.Width) {
			coco_x += 3;
		} else if (receiver.isKeyDown(irr::KEY_LEFT) && coco_x > 0) {
			coco_x -= 3;
		}

		if (device->getTimer()->getTime() - color_update >= 500) {
			background = randomColor(device->getRandomizer());
			color_update = device->getTimer()->getTime();
		}

		// render everything on screen
		driver->beginScene(true, true, background);
		guienv->drawAll();
		driver->draw2DImage(coco, irr::core::position2d<irr::s32>(coco_x, coco_y),
			irr::core::rect<irr::s32>(0, 0, 100, 250), 0, irr::video::SColor(0, 255, 255, 255), true);
		driver->draw2DImage(logo, irr::core::position2d<irr::s32>(receiver.getMouseX(), receiver.getMouseY()),
			irr::core::rect<irr::s32>(0, 0, 700, 100), 0, irr::video::SColor(0, 255, 255, 255), true);
		driver->endScene();
	}
	device->drop();
	return 0;
}
