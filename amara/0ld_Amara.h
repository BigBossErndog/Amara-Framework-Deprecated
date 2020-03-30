#ifndef AMARA_H
#define AMARA_H

/*
 * The Amara framework is a game framework built on top of SDL to make games easier to make.
 * The Amara Engine is made by Ernest Placido a.k.a 'BigBossErndog', 2018.
 */


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <SDL_FontCache.c>

#include <stdio.h>
#include <stdlib.h>
#include <std::string.h>
#include <time.h>
#include <limits>

#include <iostream>
#include <math.h>
#include <nlohmann/nlohmann::json.hpp>

namespace FAKE_AMARA {
	class WorldEntity;
	class Game;
	class LinkedItem;
	class LinkedList;
	class HashMap;
	class Asset;
	class Loader;
	class Scene;
	class Layer;
	class Camera;
	class Container;
	class Sprite;
	class ObjectManager;
	class SceneObjectManager;
	class SceneManager;
	class SceneStarter;
	class SFX;
	class Music;
	class Mouse;
	class Keyboard;
	class Director;
	class Actor;
	class Act;
	class Script;

	int makePath(void*);

	const SDL_BlendMode BLENDMODE_ADD = SDL_BLENDMODE_ADD;
	const SDL_BlendMode BLENDMODE_ALPHASUBTRACT = SDL_ComposeCustomBlendMode(
		SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
		SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT
	);

	/*
	 * Used to crash the program on purpose, for testing.
	 */
	int breakGame() {
		SDL_Rect* item = NULL;
		return item->w;
	}

	void startRand() {
		srand(time(NULL));
	}

	void startRand(int seed) {
		srand(seed);
	}

	void startRand(time_t* newtime) {
		srand(time(newtime));
	}

	int intRand(int max) {
		return rand()%max;
	}

	int intRand(int min, int max) {
		return min + (rand()%(max-min));
	}

	float floatRand(float max) {
		return ((float)rand()/(float)RAND_MAX) * max;
	}

	float floatRand(float min, float max) {
		return min + ((float)rand()/(float)RAND_MAX)*(max-min);
	}

	/*
	 * Used to hold a 2d std::vector of ints.
	 */
	class intVector {
		public:
			int x = 0;
			int y = 0;

			intVector() {

			}

			intVector(int startx, int starty) {
				x = startx;
				y = starty;
			}

			void set(int newx, int newy) {
				x = newx;
				y = newy;
			}
	};

	/*
	 * Used to hold a 2d std::vector of floats
	 */
	class floatVector {
		public:
			float x = 0;
			float y = 0;

			floatVector() {

			}

			floatVector(float startx, float starty) {
				x = startx;
				y = starty;
			}

			void set(float newx, float newy) {
				x = newx;
				y = newy;
			}
	};

	/*
	 * Used to hold an x, y, width and height rectangles of ints.
	 */
	class intRectangle {
		public:
			int x = 0;
			int y = 0;
			int width = 0;
			int height = 0;

			intRectangle() {

			}

			intRectangle(int newx, int newy, int newwidth, int newheight) {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
			}

			void set(int newx, int newy, int newwidth, int newheight) {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
			}
	};

	/*
	 * Used to hold an x, y, width and height rectangles of floats.
	 */
	class floatRectangle {
		public:
			float x = 0;
			float y = 0;
			float width = 0;
			float height = 0;

			floatRectangle() {

			}

			floatRectangle(float newx, float newy, float newwidth, float newheight) {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
			}

			void set(float newx, float newy, float newwidth, float newheight) {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
			}
	};

	/*
	 * Used to hold objects linked together in a LinkedList.
	 */
	class LinkedItem {
		public:
			void* item = NULL;
			Amara::LinkedItem* next = NULL;

			void destroy() {
				delete item;
				item = NULL;
			}
	};

	/*
	 * A base for other comparators that compares two objects.
	 */
	class Comparator {
		public:
			virtual int compare(void* object1, void* object2) = 0;
	};

	/*
	 * Holds a linked list of objects.
	 */
	class LinkedList {
		public:
			Amara::LinkedItem* startItem = NULL;

			LinkedList() {

			}

			int push(void* item) {
				Amara::LinkedItem* litem = new Amara::LinkedItem();
				litem->item = item;

				if (startItem == NULL) {
					startItem = litem;
				}
				else {
					Amara::LinkedItem* curItem = startItem;
					while (curItem->next != NULL) {
						curItem = curItem->next;
					}
					curItem->next = litem;
				}

				return 1;
			}

			void* get(int index) {
				if (startItem == NULL) return NULL;
				int count = 0;
				Amara::LinkedItem* curItem = startItem;

				while (count != index && curItem != NULL) {
					count++;
					curItem = curItem->next;
				}

				return curItem->item;
			}

			void* remove(int index) {
				if (startItem == NULL) return NULL;
				int counter = 0;
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* lastItem = NULL;

				while (counter != index && curItem != NULL) {
					lastItem = curItem;
					curItem = curItem->next;
					counter += 1;
				}

				if (curItem != NULL && counter == index) {

					if (lastItem == NULL) {
						startItem = curItem->next;
					}
					else {
						lastItem->next = curItem->next;
					}

					void* toReturn = curItem->item;
					delete curItem;

					return toReturn;
				}

				return NULL;
			}

			void* remove(void* item) {
				if (startItem == NULL) return NULL;
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* lastItem = NULL;

				while (curItem->item != item && curItem != NULL) {
					lastItem = curItem;
					curItem = curItem->next;
				}

				if (curItem != NULL) {

					if (lastItem == NULL) {
						startItem = curItem->next;
					}
					else {
						lastItem->next = curItem->next;
					}

					void* toReturn = curItem->item;
					delete curItem;

					return toReturn;
				}

				return NULL;
			}

			bool contains(void* item) {
				if (startItem == NULL) return false;
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* lastItem = NULL;

				while (curItem != NULL) {
					if (curItem->item == item) {
						return true;
					}
					lastItem = curItem;
					curItem = curItem->next;
				}

				return false;
			}

			void* getStartItem() {
				if (startItem == NULL) return NULL;
				return startItem->item;
			}

			void* getLast() {
				if (startItem == NULL) return NULL;
				Amara::LinkedItem* curItem = startItem;

				while (curItem->next != NULL) {
					curItem = curItem->next;
				}

				return curItem;
			}

			int length() {
				Amara::LinkedItem* curItem = startItem;
				int count = 0;

				while (curItem != NULL) {
					curItem = curItem->next;
					count += 1;
				}

				return count;
			}

			// Performs a bubble sort on the linked list.
			Amara::LinkedItem* sort(Amara::Comparator* comparator, bool descending) {
				if (startItem == NULL) return NULL;
				if (startItem->next == NULL) return NULL;

				bool swapped = false;
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* nextItem = curItem->next;
				Amara::LinkedItem* prevItem = NULL;

				do {
					swapped = false;
					curItem = startItem;
					nextItem = curItem->next;

					while (nextItem != NULL) {
						int comparison = comparator->compare(curItem->item, nextItem->item);
						if (comparison > 0) {
							if (curItem == startItem) {
								startItem = nextItem;
							}
							else {
								prevItem->next = nextItem;
							}
							curItem->next = nextItem->next;
							nextItem->next = curItem;

							prevItem = nextItem;
							nextItem = curItem->next;

							swapped = true;
						}
						else {
							prevItem = curItem;
							curItem = curItem->next;
							nextItem = curItem->next;
						}
					}
				} while (swapped);

				return startItem;
			}

			Amara::LinkedItem* sort(Amara::Comparator* comparator) {
				return sort(comparator, false);
			}

			void destroy() {
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* nextItem;

				while (curItem != NULL) {
					nextItem = curItem->next;
					delete curItem;
					curItem = nextItem;
				}

				startItem = NULL;
			}

			void deepDestroy() {
				Amara::LinkedItem* curItem = startItem;
				Amara::LinkedItem* nextItem;

				while (curItem != NULL) {
					nextItem = curItem->next;
					curItem->destroy();
					delete curItem;
					curItem = nextItem;
				}

				startItem = NULL;
			}
	};

	/*
	 * Used to hold a item in a HashMap.
	 */
	class HashItem {
		public:
			char* key = NULL;
			void* item = NULL;

			HashItem(char* newKey, void* newItem) {
				key = newKey;
				item = newItem;
			}

			destroy() {
				delete key;
				delete item;
				key = NULL;
				item = NULL;
			}
	};

	/*
	 * Used to hold a HashMap of objects with keys.
	 */
	class HashMap {
		public:
			Amara::LinkedList** buckets = NULL;

			HashMap(int startNum) {
				numBuckets = startNum;
				init();
			}

			int makeHash(char* key) {
				int count = 0;
				int hash = 0;
				while (key[count] != '\0') {
					hash += key[count] + 2;
					count += 1;
				}

				hash = hash % numBuckets;
				return hash;
			}

			bool add(char* key, void* item, bool replace) {
				int hash = makeHash(key);

				Amara::HashItem* hashitem = NULL;
				if (buckets[hash] == NULL) {
					buckets[hash] = new Amara::LinkedList();
				}
				else {
					hashitem = getHashItem(key);
					if (hashitem != NULL) {
						if (replace) {
							hashitem->item = item;
							return true;
						}
						else {
							return false;
						}
					}
				}

				Amara::HashItem* newItem = new Amara::HashItem(key, item);
				buckets[hash]->push(newItem);

				return true;
			}

			bool add(char* key, void* item) {
				return add(key, item, true);
			}

			void* get(char* key) {
				int hash = makeHash(key);
				if (buckets[hash] == NULL) return NULL;
				Amara::LinkedItem* item = buckets[hash]->startItem;
				Amara::HashItem* hashedItem = NULL;

				if (item != NULL) {
					hashedItem = (Amara::HashItem*) item->item;
				}

				while (item != NULL && strcmp(key, hashedItem->key) != 0) {
					item = item->next;
					if (item != NULL) {
						hashedItem = (Amara::HashItem*) item->item;
					}
				}

				if (item == NULL) {
					return NULL;
				}
				else {
					return hashedItem->item;
				}
			}

			Amara::HashItem* getHashItem(char* key) {
				int hash = makeHash(key);
				if (buckets[hash] == NULL) return NULL;
				Amara::LinkedItem* item = buckets[hash]->startItem;
				Amara::HashItem* hashedItem = NULL;

				if (item != NULL) {
					hashedItem = (Amara::HashItem*) item->item;
				}

				while (item != NULL && strcmp(key, hashedItem->key) != 0) {
					item = item->next;
					if (item != NULL) {
						hashedItem = (Amara::HashItem*) item->item;
					}
				}

				if (item == NULL) {
					return NULL;
				}
				else {
					return hashedItem;
				}
			}

			void* remove(char* key) {
				int hash = makeHash(key);
				if (buckets[hash] == NULL) return NULL;
				Amara::LinkedItem* item = buckets[hash]->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::HashItem* hashedItem = NULL;

				if (item != NULL) {
					hashedItem = (Amara::HashItem*) item->item;
				}

				while (item != NULL && strcmp(key, hashedItem->key) != 0) {
					lastItem = item;
					item = item->next;
					if (item == NULL) {
						hashedItem = (Amara::HashItem*) item->item;
					}
				}

				if (item == NULL) {
					return NULL;
				}
				else {
					if (lastItem == NULL) {
						buckets[hash]->startItem = item->next;
					}
					else {
						lastItem->next = item->next;
					}
					delete item;

					void* toReturn = hashedItem->item;
					delete hashedItem;
					return toReturn;
				}
			}

			Amara::LinkedList* getKeys() {
				Amara::LinkedList* keys = new Amara::LinkedList();

				for (int i = 0; i < numBuckets; i++) {
					if (buckets[i] !=  NULL) {
						Amara::LinkedItem* curItem = buckets[i]->startItem;

						while (curItem != NULL) {
							keys->push(((Amara::HashItem*) curItem->item)->key);
							curItem = curItem->next;
						}
					}
				}

				return keys;
			}

			void setNumBuckets(int newNum) {
				numBuckets = newNum;
			}

			void init() {
				destroy();

				buckets = new Amara::LinkedList*[numBuckets];

				for (int i = 0; i < numBuckets; i++) {
					buckets[i] = NULL;
				}
			}

			void destroy() {
				Amara::LinkedList* curList;
				int curBucket = 0;

				if (buckets == NULL) return;

				while (curBucket < numBuckets) {
					curList = buckets[curBucket];
					if (curList != NULL) {
						curList->destroy();
						delete curList;
					}
					curBucket++;
				}

				delete buckets;
			}

			void deepDestroy() {
				Amara::LinkedList* curList;
				int curBucket = 0;

				if (buckets == NULL) return;

				while (curBucket < numBuckets) {
					curList = buckets[curBucket];
					if (curList != NULL) {
						Amara::LinkedItem* curItem = curList->startItem;
						while (curItem != NULL) {
							((Amara::HashItem*) curItem->item)->destroy();
							curItem = curItem->next;
						}

						curList->deepDestroy();
						delete curList;
					}
					curBucket++;
				}

				delete buckets;
			}

		protected:
			int numBuckets = 10;
	};

	/*
	 * Combines a LinkedList with HashMap
	 */
	class LinkedHashMap {
		public:
			Amara::LinkedList* listKeys = NULL;
			Amara::LinkedList* listItems = NULL;
			Amara::HashMap* hashMap = NULL;

			LinkedHashMap(int size) {
				listKeys = new Amara::LinkedList();
				listItems = new Amara::LinkedList();
				hashMap = new Amara::HashMap(size);
			}

			void add(char* key, void* item, bool replace) {
				listKeys->push(key);
				listItems->push(item);
				hashMap->add(key, item, replace);
			}

			void add(char* key, void* item) {
				add(key, item, true);
			}

			void* get(char* key) {
				return hashMap->get(key);
			}

			Amara::LinkedItem* getStart() {
				return listItems->startItem;
			}

			void* remove(char* key) {
				void* item = hashMap->get(key);
				if (item == NULL) return NULL;

				hashMap->remove(key);
				listItems->remove(item);
				listKeys->remove(key);
			}

			void destroy() {
				listKeys->destroy();
				listItems->destroy();
				hashMap->destroy();

				delete listKeys;
				delete listItems;
				delete hashMap;
			}

			void deepDestroy() {
				listKeys->destroy();
				listItems->destroy();
				hashMap->deepDestroy();

				delete listKeys;
				delete listItems;
				delete hashMap;
			}
	};

	class BinaryNode {
		public:
			BinaryNode* left = NULL;
			BinaryNode* right = NULL;

			void* item = NULL;

			BinaryNode(void* newitem) {
				item = newitem;
			}

			bool add(Amara::Comparator* comparator, void* newitem) {
				if (comparator->compare(newitem, item) < 0) {
					if (left == NULL) {
						left = new BinaryNode(newitem);
						return true;
					}
					return left->add(comparator, newitem);
				}
				else {
					if (right == NULL) {
						right = new BinaryNode(newitem);
						return true;
					}
					return right->add(comparator, newitem);
				}
			}
	};

	/*
	 * Holds data in a sorted tree structure.
	 */
	class BinaryTree {
		public:
			Comparator* comparator = NULL;

			BinaryNode* root = NULL;

			BinaryTree(Comparator* newcomparator) {
				comparator = newcomparator;
			}

			bool add(void* item) {
				if (item == NULL) return false;

				if (root == NULL) {
					root = new BinaryNode(item);
					return true;
				}

				root->add(comparator, item);
			}
	};

	/*
	 * The application time based timer
	 */
	class LTimer {
		public:
			LTimer() {
				mStartTicks = 0;
				mPausedTicks = 0;

				mPaused = false;
				mStarted = false;
			}

			void start() {
				mStarted = true;
				mPaused = false;

				mStartTicks = SDL_GetTicks();
				mPausedTicks = 0;
			}

			void stop() {
				mStarted = false;
				mPaused = false;

				mStartTicks = 0;
				mPausedTicks = 0;
			}

			void pause() {
				if (mStarted && !mPaused) {
					mPaused = true;

					mPausedTicks = SDL_GetTicks() - mStartTicks;
					mStartTicks = 0;
				}
			}

			void unpause() {
				if (mStarted && mPaused) {
					mPaused = false;

					mStartTicks = SDL_GetTicks() - mPausedTicks;

					mPausedTicks = 0;
				}
			}

			Uint32 getTicks() {
				Uint32 time = 0;

				if (mStarted) {
					if (mPaused) {
						time = mPausedTicks;
					}
					else {
						time = SDL_GetTicks() - mStartTicks;
					}
				}

				return time;
			}

			bool isStarted() {
				return mStarted;
			}

			bool isPaused() {
				return mPaused && mStarted;
			}

		private:
			// Clock time when the timer started
			Uint32 mStartTicks;
			// The ticks stored when the timer was paused
			Uint32 mPausedTicks;
			// the timer statues
			bool mPaused;
			bool mStarted;
	};

	/*
	 * Holds all the values of a key.
	 */
	class Key {
		public:
			bool destroyed = false;

			char* key = NULL;
			SDL_Keycode keycode = 0;
			bool isDown = false;
			bool tapped = false;
			bool released = false;
			bool pressed = false;
			bool activated = false;
			bool held = false;
			int downCounter = 0;
			int tapTime = 15;

			bool isStick = false;
			Amara::floatVector* recpushforce = NULL;
			Amara::floatVector* pushforce = NULL;
			float pushSensitivity = 0.2;
			Key* up = NULL;
			Key* down = NULL;
			Key* left = NULL;
			Key* right = NULL;

			bool pressureSensitive = false;
			float pressure = 0;
			float recPressure = 0;
			float  pressureSensitivity = 0.8;

			Key() {
				downCounter = 0;
			}

			Key(char* keyname): Key() {
				key = keyname;
			}

			Key(char* keyname, SDL_Keycode newkeycode): Key() {
				key = keyname;
				keycode = newkeycode;
			}

			void initStick() {
				recpushforce = new Amara::floatVector(0, 0);
				pushforce = new Amara::floatVector(0, 0);
				up = new Key();
				down = new Key();
				left = new Key();
				right = new Key();
				isStick = true;
			}

			void initPressure() {
				pressureSensitive = true;
			}

			void manage() {
				released = false;
				tapped = false;
				pressed = false;
				activated = false;
				if (isDown) {
					downCounter += 1;
					if (downCounter > tapTime) {
						held = true;
					}
				}

				if (isStick) {
					up->manage();
					down->manage();
					left->manage();
					right->manage();

					if (pushforce->y < -pushSensitivity && recpushforce->y > -pressureSensitivity) {
						up->press();
					}
					else if (pushforce->y > -pushSensitivity && recpushforce->y < -pushSensitivity) {
						up->release();
					}

					if (pushforce->y > pushSensitivity && recpushforce->y < pressureSensitivity) {
						down->press();
					}
					else if (pushforce->y < pushSensitivity && recpushforce->y > pushSensitivity) {
						down->release();
					}

					if (pushforce->x > pushSensitivity && recpushforce->x < pressureSensitivity) {
						right->press();
					}
					else if (pushforce->x < pushSensitivity && recpushforce->x > pushSensitivity) {
						right->release();
					}

					if (pushforce->x < -pushSensitivity && recpushforce->x > -pressureSensitivity) {
						left->press();
					}
					else if (pushforce->x > -pushSensitivity && recpushforce->x < -pushSensitivity) {
						left->release();
					}

					recpushforce->x = pushforce->x;
					recpushforce->y = pushforce->y;
				}

				if (pressureSensitive) {
					if (pressure > pressureSensitivity && recPressure < pressureSensitivity) {
						press();
					}
					else if (pressure < pressureSensitivity && recPressure > pressureSensitivity) {
						release();
					}
					recPressure = pressure;
				}
			}

			void press() {
				if (!isDown) {
					isDown = true;
					downCounter = 0;
					pressed = true;
					held = false;
				}
				activated = true;
			}

			void release() {
				released = true;
				pressed = false;
				isDown = false;
				activated = false;
				held = false;
				if (downCounter < tapTime) {
					tapped = true;
				}
			}

			bool is(char *keycheck) {
				if (strcmp(key, keycheck) == 0) {
					return true;
				}
				else {
					return false;
				}
			}

			void destroy() {
				if (destroyed) return;
				free(key);
				if (isStick) {
					delete recpushforce;
					delete pushforce;
					delete up;
					delete down;
					delete left;
					delete right;
				}
			}

			~Key() {
				destroy();
			}
	};

	class KeyGroup {
		public:
			Amara::Key* up = NULL;
			Amara::Key* down = NULL;
			Amara::Key* left = NULL;
			Amara::Key* right = NULL;

			Amara::Key* w = NULL;
			Amara::Key* s = NULL;
			Amara::Key* a = NULL;
			Amara::Key* d = NULL;

			Amara::Key* space = NULL;
	};

	/*
	 * Manages all the key presses.
	 */
	class Keyboard {
		public:
			Amara::LinkedHashMap* keys = NULL;
			Amara::Key* lastPressed = NULL;
			Amara::Key* lastReleased = NULL;

			Keyboard() {
				keys = new Amara::LinkedHashMap(20);
			}

			Amara::Key* addKey(char* key, SDL_Keycode keycode) {
				Amara::Key* newKey = new Amara::Key(key, keycode);
				keys->add(key, newKey);
				return newKey;
			}

			Amara::Key* getKey(char* key) {
				return (Amara::Key*) keys->get(key);
			}

			void manage() {
				lastPressed = NULL;
				lastReleased = NULL;
				Amara::LinkedItem* curItem = keys->getStart();

				while (curItem != NULL) {
					Amara::Key* curKey = (Amara::Key*) curItem->item;

					curKey->manage();

					curItem = curItem->next;
				}
			}

			void press(SDL_Keycode keycode) {
				Amara::LinkedItem* curItem = keys->getStart();

				while (curItem != NULL) {
					Amara::Key* curKey = (Amara::Key*) curItem->item;

					if (curKey->keycode == keycode) {
						curKey->press();
						lastPressed = curKey;
					}

					curItem = curItem->next;
				}
			}

			void release(SDL_Keycode keycode) {
				Amara::LinkedItem* curItem = keys->getStart();

				while (curItem != NULL) {
					Amara::Key* curKey = (Amara::Key*) curItem->item;

					if (curKey->keycode == keycode) {
						curKey->release();
						lastReleased = curKey;
					}

					curItem = curItem->next;
				}
			}

			Amara::KeyGroup* createArrowKeys(Amara::KeyGroup* existing) {
				Amara::KeyGroup* newkeys = existing;
				if (newkeys == NULL) {
					newkeys = new Amara::KeyGroup();
				}

				newkeys->up = addKey("up", SDLK_UP);
				newkeys->down = addKey("down", SDLK_DOWN);
				newkeys->left = addKey("left", SDLK_LEFT);
				newkeys->right = addKey("right", SDLK_RIGHT);

				return newkeys;
			}

			Amara::KeyGroup* createArrowKeys() {
				return createArrowKeys(NULL);
			}

			Amara::KeyGroup* createWASD(Amara::KeyGroup* existing) {
				Amara::KeyGroup* newkeys = existing;
				if (newkeys == NULL) {
					newkeys = new Amara::KeyGroup();
				}

				newkeys->w = addKey("w", SDLK_w);
				newkeys->s = addKey("s", SDLK_s);
				newkeys->a = addKey("a", SDLK_a);
				newkeys->d = addKey("d", SDLK_d);

				return newkeys;
			}

			Amara::KeyGroup* createWASD() {
				return createWASD(NULL);
			}

			Amara::Key* createSpace(Amara::KeyGroup* existing) {
				Amara::Key* spaceKey = addKey("space", SDLK_SPACE);

				if (existing != NULL) {
					existing->space = spaceKey;
				}

				return spaceKey;
			}

			Amara::Key* createSpace() {
				return createSpace(NULL);
			}
	};

	/*
	 * Manages the mouse.
	 */
	class Mouse: public Amara::intVector {
		public:
			Key* leftButton = NULL;
			Key* rightButton = NULL;

			Mouse(): Amara::intVector(0, 0) {
				leftButton = new Key("leftMouseButton");
				rightButton = new Key("rightMouseButton");
			}

			void manage() {
				leftButton->manage();
				rightButton->manage();
			}
	};

	/*
	 * Manages a controller.
	 */
	class Controller {
		public:
			SDL_GameController* controller = NULL;
			bool destroyed = false;

			Key* lastPressed = NULL;

			Key* start = NULL;
			Key* select = NULL;
			Key* home = NULL;

			Key* lt = NULL;
			Key* rt = NULL;

			Key* lb = NULL;
			Key* rb = NULL;

			Key* x = NULL;
			Key* y = NULL;
			Key* a = NULL;
			Key* b = NULL;

			Key* ls = NULL;
			Key* rs = NULL;

			Key* up = NULL;
			Key* down = NULL;
			Key* left = NULL;
			Key* right = NULL;

			Controller() {
				start = new Key();
				select = new Key();
				home = new Key();

				lt = new Key();
				rt = new Key();
				lt->initPressure();
				rt->initPressure();

				lb = new Key();
				rb = new Key();

				x = new Key();
				y = new Key();
				a = new Key();
				b = new Key();

				ls = new Key();
				rs = new Key();
				ls->initStick();
				rs->initStick();

				up = new Key();
				down = new Key();
				left = new Key();
				right = new Key();
			}

			Controller(SDL_GameController* newcontroller): Controller() {
				controller = newcontroller;
			}

			void manage() {
				start->manage();
				select->manage();
				home->manage();

				lt->manage();
				rt->manage();

				lb->manage();
				rb->manage();

				x->manage();
				y->manage();
				a->manage();
				b->manage();

				ls->manage();
				rs->manage();

				up->manage();
				down->manage();
				left->manage();
				right->manage();
			}

			void press(Uint8 button) {
				switch (button) {
					case SDL_CONTROLLER_BUTTON_A:
						a->press();
						lastPressed = a;
						break;
					case SDL_CONTROLLER_BUTTON_B:
						b->press();
						lastPressed = b;
						break;
					case SDL_CONTROLLER_BUTTON_X:
						x->press();
						lastPressed = x;
						break;
					case SDL_CONTROLLER_BUTTON_Y:
						y->press();
						lastPressed = y;
						break;
					case SDL_CONTROLLER_BUTTON_BACK:
						select->press();
						lastPressed = select;
						break;
					case SDL_CONTROLLER_BUTTON_GUIDE:
						home->press();
						lastPressed = home;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSTICK:
						ls->press();
						lastPressed = ls;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
						rs->press();
						lastPressed = rs;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
						lb->press();
						lastPressed = lb;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
						rb->press();
						lastPressed = rb;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						up->press();
						lastPressed = up;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						down->press();
						lastPressed = down;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						left->press();
						lastPressed = left;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						right->press();
						lastPressed = right;
						break;
				}
			}

			void release(Uint8 button) {
				switch (button) {
					case SDL_CONTROLLER_BUTTON_A:
						a->release();
						break;
					case SDL_CONTROLLER_BUTTON_B:
						b->release();
						break;
					case SDL_CONTROLLER_BUTTON_X:
						x->release();
						break;
					case SDL_CONTROLLER_BUTTON_Y:
						y->release();
						break;
					case SDL_CONTROLLER_BUTTON_BACK:
						select->release();
						break;
					case SDL_CONTROLLER_BUTTON_GUIDE:
						home->release();
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSTICK:
						ls->release();
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
						rs->release();
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
						lb->release();
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
						rb->release();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						up->release();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						down->release();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						left->release();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						right->release();
						break;
				}
			}

			void destroy() {
				if (destroyed) return;
				destroyed = true;
				delete start;
				delete select;
				delete home;

				delete lt;
				delete rt;

				delete lb;
				delete rb;

				delete x;
				delete y;
				delete a;
				delete b;

				delete ls;
				delete rs;
			}
	};

	/*
	 * Manages a joystick.
	 */
	class Joystick {
		public:
			SDL_Joystick* joystick = NULL;

			Joystick() {}

			Joystick(SDL_Joystick* newjoystick) {
				joystick = newjoystick;
			}
	};

	/*
	 * Used to bind control input.
	 */
	class ControlScheme {
		public:
			Amara::HashMap* controls = NULL;
			bool destroyed = false;

			ControlScheme() {
				controls = new Amara::HashMap(10);
			}

			Key* assign(char* assignkey, Key* key) {
				if (destroyed) return NULL;
				controls->add(assignkey, key);
				return key;
			}

			Key* get(char* getkey) {
				if (destroyed) return NULL;
				return (Key*) controls->get(getkey);
			}

			void destroy() {
				if (destroyed) return;
				controls->destroy();
				controls = NULL;
				destroyed = true;
			}
	};

	/*
	 * Holds all the properties of the game.
	 */
	class GameProperties {
		public:
			SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;
			SDL_Texture* curTexture = NULL;

			Amara::Loader* load = NULL;

			Amara::intRectangle* display = NULL;
			Amara::intRectangle* resolution = NULL;
			Amara::intRectangle* window = NULL;

			int curAlpha = 1;

			Amara::SceneManager* sceneManager = NULL;
			Amara::SceneStarter* scenes = NULL;

			Amara::intVector* camera = new Amara::intVector(0, 0);
			Amara::floatVector* camScale = new Amara::floatVector(1, 1);

			Amara::Keyboard* keyboard = NULL;
			Amara::Mouse* mouse = NULL;
			Amara::LinkedList* controllers = NULL;

			Amara::Music* music = NULL;

			Amara::LinkedHashMap* storage = NULL;

			Amara::WorldEntity* player = NULL;

			Amara::ControlScheme* controlScheme = NULL;

			bool lagging = false;

			bool vsync;
			int fps;
			int tps;
			int lps;

			int width;
			int height;

			bool quit = NULL;
			bool dragged = false;
			bool windowFocused = true;

			void closeGame() {
				quit = true;
			}

			void setFPS(int newFps, bool lockLogicSpeed) {
				fps = newFps;
				tps = 1000 / fps;
				if (!lockLogicSpeed) {
					lps = newFps;
				}
			}

			void setFPS(int newFps) {
				setFPS(newFps, false);
			}

			void setFPS(int newFps, int newLps) {
				fps = newFps;
				lps = newLps;
				tps = 1000 / fps;
			}

			void setLogicTickRate(int newRate) {
				lps = newRate;
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				if (gRenderer != NULL) {
					SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
				}
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
				if (gRenderer != NULL) {
					SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
				}
			}

			void resizeWindow(int neww, int newh) {
				if (gWindow != NULL) {
					SDL_SetWindowSize(gWindow, neww, newh);
					window->width = neww;
					window->height = newh;
					width = window->width;
					height = window->height;
				}
			}

			void setWindowPosition(int newx, int newy) {
				if (gWindow != NULL) {
					SDL_SetWindowPosition(gWindow, newx, newy);
					window->x = newx;
					window->y = newy;
				}
			}

			void setResolution(int neww, int newh) {
				if (gRenderer != NULL) {
					SDL_RenderSetLogicalSize(gRenderer, neww, newh);
				}
				resolution->width = neww;
				resolution->height = newh;
			}

			void windowedFullScreen() {
				resizeWindow(display->width, display->height);
				setWindowPosition(0, 0);
			}

			Amara::Controller* getController(int index) {
				return (Amara::Controller*) controllers->get(index);
			}

			int numControllers() {
				return controllers->length();
			}
	};

	class Director {
		public:
			char* act = NULL;
			int actionCounter = 0;
			int currentAction = 0;
			bool cancelAction = false;
			char* jumpFlag = NULL;
	};

	/*
	 * Can be applied to an Actor so that it will run it.
	 */
	class Script {
		public:
			bool paused = false;
			bool destroyed = false;

			Script() {
				reset();
			}

			void run(Amara::Actor* entity) {
				if (paused || destroyed) return;
				update(entity);
			}

			virtual void update(Amara::Actor* entity) {

			}

			void resume() {
				paused = false;
			}

			void pause() {
				paused = true;
			}

			void reset() {
				paused = false;
			}

			void destroy() {
				if (destroyed) return;
				destroyed = true;
			}
	};

	/*
	 * Handles object states.
	 */
	class Actor {
		public:
			Amara::Director* director = NULL;
			Amara::Director* myDirector = NULL;

			Amara::LinkedList* scripts = NULL;

			virtual void setDirector(Amara::Director* newDirector) {
				director = newDirector;
			}

			virtual void setDirector(Amara::Actor* newDirector) {
				director = newDirector->director;
			}

			void actWith(Actor* otherActor) {
				director = otherActor->director;
			}

			virtual void becomeDirector() {
				director = new Amara::Director;
				myDirector = director;
			}

			virtual void run() {
				if (myDirector == NULL) return;
				director->actionCounter = 0;
				director->cancelAction = false;

				runScripts();
			}

			void runScripts() {
				if (scripts == NULL) return;

				Amara::LinkedItem* curItem = scripts->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				while (curItem != NULL) {
					Amara::Script* curScript = (Amara::Script*) curItem->item;
					if (!curScript->destroyed) {
						curScript->run(this);
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							scripts->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}

			virtual void add(Amara::Script* script) {
				if (script == NULL) return;

				if (scripts == NULL) {
					scripts = new Amara::LinkedList();
				}

				scripts->push(script);
			}

			virtual bool action() {
				if (director == NULL) return false;
				bool toReturn = false;
				if (director->actionCounter == director->currentAction && !director->cancelAction) {
					toReturn = true;
				}
				director->actionCounter += 1;
				return toReturn;
			}

			virtual void nextAction() {
				if (director == NULL) return;
				director->currentAction += 1;
				director->cancelAction = true;
			}

			virtual bool act(char* key) {
				if (director == NULL) return false;
				if (director->cancelAction) return false;
				if (strcmp(key, director->act) == 0) {
					return true;
				}
				return false;
			}

			virtual void switchState(char* key) {
				if (director == NULL) return;
				director->act = key;
				director->currentAction = 0;
			}

			virtual void jump(char* key) {
				if (director == NULL) return;
				director->jumpFlag = key;
				director->cancelAction = true;
			}

			virtual bool jumpAction(char* key) {
				if (director == NULL) return false;
				if (action()) {
					jump(key);
					return true;
				}
			}

			virtual bool bookmark(char* key) {
				if (director == NULL) return false;
				if (action() && director->jumpFlag != NULL) {
					if (strcmp(key, director->jumpFlag) == 0) {
						director->currentAction = director->actionCounter;
						nextAction();
						director->jumpFlag = NULL;
					}
					return true;
				}
				return false;
			}

			virtual void destroy() {
				if (director != NULL && myDirector == director) {
					delete director;
				}

				if (scripts != NULL) {
					scripts->destroy();
					delete scripts;
				}
			}
	};

	/*
	 * Used to bind acts to a director.
	 * Please write Acts in the create function using the writeAct function.
	 * When you add this to a Scene or WorldEntity it automatically adds all the Acts to the scene.
	 */
	class Playwright {
		public:
			Amara::LinkedList* allacts = NULL;
			Amara::LinkedItem* curItem = NULL;

			Playwright() {
				allacts = new Amara::LinkedList();
			}

			virtual void create() = 0;
			writeAct(Amara::Act* newact) {
				allacts->push(newact);
			}

			Amara::Act* read() {
				if (curItem == NULL) {
					curItem = allacts->startItem;
				}
				else {
					curItem = curItem->next;
				}

				if (curItem != NULL) {
					return (Amara::Act*)curItem->item;
				}
				return NULL;
			}
	};

	/*
	 * The backbone of every object in the game.
	 */
	class WorldEntity: public Amara::Actor {
		public:
			Amara::GameProperties* game = NULL;
			Amara::Loader* load = NULL;
			Amara::LinkedList* entities = NULL;
			Amara::WorldEntity* parent = NULL;

			SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

			bool destroyed = false;
			float x = 0;
			float y = 0;
			int z = 0;
			float width = 0;
			float height = 0;

			bool physicsEnabled = false;

			Amara::floatVector* scale = NULL;
			Amara::floatVector* anchor = NULL;

			Amara::floatVector* vel = NULL;
			Amara::floatVector* friction = NULL;
			Amara::floatVector* checkSpacing = NULL;

			bool shown = true;
			bool culled = false;
			double alpha = 1;

			bool paused = false;

			bool inheritEntities = false;

			int distanceToGoal = 0;

			Amara::intRectangle* drawRect = NULL;

			virtual void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				runPhysics();
				update();
				runEntities();
			}

			virtual void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed) return;
				drawEntities(parentx, parenty, parentwidth, parentheight);
			}

			virtual void add(Amara::Script* script) {
				Amara::Actor::add(script);
			}

			virtual Amara::WorldEntity* add(Amara::Playwright* newPlaywright) {
				Amara::Act* curAct = newPlaywright->read();
				while (curAct != NULL) {
					add((Amara::WorldEntity*)curAct);
					newPlaywright->read();
				}
				return NULL;
			}

			virtual Amara::WorldEntity* add(Amara::Camera* newCamera) {

			}

			virtual Amara::WorldEntity* add(Amara::WorldEntity* newEntity) {
				if (entities == NULL) {
					entities = new Amara::LinkedList();
				}

				newEntity->game = game;
				if (newEntity->inheritEntities) newEntity->entities = entities;
				newEntity->load = load;
				newEntity->parent = this;
				newEntity->setDirector(this);
				newEntity->preload();
				newEntity->create();

				entities->push(newEntity);
				return newEntity;
			}

			virtual Amara::WorldEntity* getRootAncestor() {
				if (parent == NULL) return NULL;
				Amara::WorldEntity* curAncestor = parent;
				while (curAncestor->parent != NULL) {
					curAncestor = curAncestor->parent;
				}

				return curAncestor;
			}

			virtual void preload() {

			}

			virtual void create() {

			}

			virtual void update() {

			}

			virtual void pause() {
				paused = true;
			}

			virtual void resume() {
				paused = false;
			}

			virtual void togglePause() {
				paused = !paused;
			}

			virtual void destroy() {
				if (destroyed) {
					return;
				}
				destroyEntities();
				destroyed = true;

				Amara::Actor::destroy();

				if (vel != NULL) {
					delete vel;
				}
				if (friction != NULL) {
					delete friction;
				}
				if (scale != NULL) {
					delete scale;
				}
				if (anchor != NULL) {
					delete anchor;
				}
				if (drawRect != NULL) {
					delete drawRect;
				}
			}

			void runEntities() {
				if (entities == NULL || destroyed || paused) return;
				Amara::LinkedItem* curItem = entities->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				while (curItem != NULL) {
					Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;
					if (!curEntity->destroyed) {
						curEntity->run();
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							entities->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}

			void drawEntities(int parentx, int parenty, int parentwidth, int parentheight) {
				if (entities == NULL || destroyed) return;
				Amara::LinkedItem* curItem = entities->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				while (curItem != NULL) {
					Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;
					if (!curEntity->destroyed) {
						curEntity->draw(parentx, parenty, parentwidth, parentheight);
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							entities->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}

			Amara::LinkedItem* sortEntities(Amara::Comparator* comparator, bool descending) {
				if (entities == NULL) return NULL;
				return entities->sort(comparator, descending);
			}

			Amara::LinkedItem* sortEntities(Amara::Comparator* comparator) {
				return sortEntities(comparator, false);
			}

			virtual void destroyEntities() {
				if (entities == NULL) return;
				Amara::LinkedItem* curItem = entities->startItem;
				while (curItem != NULL) {
					Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;

					curEntity->destroy();

					curItem = curItem->next;
				}

				entities->deepDestroy();

				entities = NULL;
			}

			virtual void goTo(float newx, float newy) {
				x = newx;
				y = newy;
			}

			virtual void setScale(float setx, float sety) {
				scale->x = setx;
				scale->y = sety;
			}

			virtual void setScale(float set) {
				scale->x = set;
				scale->y = set;
			}

			virtual void setAnchor(float setx, float sety) {
				anchor->x = setx;
				anchor->y = sety;
			}

			virtual void setAnchor(float set) {
				anchor->x = set;
				anchor->y = set;
			}

			void setBlendMode(SDL_BlendMode newmode) {
				blendMode = newmode;
			}

			virtual void actSetup() {

			}

			virtual bool fadeOut(float speed, float target) {
				if (action()) {
					if (alpha > target) {
						alpha -= speed;
					}
					else {
						alpha = target;
						nextAction();
					}
					return true;
				}
				return false;
			}

			virtual bool fadeOut(float speed) {
				return fadeOut(speed, 0);
			}

			virtual bool fadeIn(float speed, float target) {
				if (action()) {
					if (alpha < target) {
						alpha += speed;
					}
					else {
						alpha = target;
						nextAction();
					}
					return true;
				}
			}

			virtual bool fadeIn(float speed) {
				return fadeIn(speed, 1);
			}

			void enablePhysics() {
				if (physicsEnabled) return;
				physicsEnabled = true;

				vel = new Amara::floatVector(0, 0);
				friction = new Amara::floatVector(0, 0);
				checkSpacing = new Amara::floatVector(1, 1);
			}

			bool runPhysics() {
				if (!physicsEnabled) return false;

				Amara::floatVector* recPos = new Amara::floatVector(x, y);

				x += vel->x;
				y += vel->y;

				vel->x = vel->x * friction->x;
				vel->y = vel->y * friction->y;

				delete recPos;

				return true;
			}

			void setFriction(float newx, float newy) {
				friction->x = newx;
				friction->y = newy;
			}

			void setVelocity(float newx, float newy) {
				vel->x = newx;
				vel->y = newy;
			}

			bool hover() {
				if (drawRect == NULL) return false;
				if (game->mouse->x > drawRect->x && game->mouse->x < drawRect->x + drawRect->width) {
					if (game->mouse->y > drawRect->y && game->mouse->y < drawRect->y + drawRect->height) {
						return true;
					}
				}

				return false;
			}

			bool clicked() {
				return (game->mouse->leftButton->pressed && hover());
			}

			bool tapped() {
				return (game->mouse->leftButton->tapped && hover());
			}

			bool held() {
				return (game->mouse->leftButton->isDown && hover());
			}

			bool drag() {
				printf("Drag not implemented.");
				return false;
			}
	};

	/*
	 * Used to manage all the actions in an act.
	 */
	class Act: public Amara::WorldEntity {
		char* actName = NULL;

		Act() {

		}

		Act(char* newname) {
			actName = newname;
		}

		void setName(char* newname) {
			actName = newname;
		}

		void run() {
			setDirector(parent);
			if (destroyed || paused) return;
			if (act(actName)) {
				update();
				script();
			}
		}

		virtual void script() {

		}
	};

	/*
	 * Compares two entities by x value.
	 */
	class XComparator: public Amara::Comparator {
		public:
			XComparator() {

			}

			virtual int compare(void* object1, void* object2) {
				Amara::WorldEntity* obj1 = (Amara::WorldEntity*) object1;
				Amara::WorldEntity* obj2 = (Amara::WorldEntity*) object2;

				return obj1->x - obj2->x;
			}
	};
	Amara::XComparator* xComparator = new Amara::XComparator();

	/*
	 * Compares two entities by y value.
	 */
	class YComparator: public Amara::Comparator {
		public:
			YComparator() {

			}

			virtual int compare(void* object1, void* object2) {
				Amara::WorldEntity* obj1 = (Amara::WorldEntity*) object1;
				Amara::WorldEntity* obj2 = (Amara::WorldEntity*) object2;

				return obj1->y - obj2->y;
			}
	};
	Amara::YComparator* yComparator = new Amara::YComparator();

	/*
	 * Compares two entities by z value.
	 */
	class ZComparator: public Amara::Comparator {
		public:
			ZComparator() {

			}

			virtual int compare(void* object1, void* object2) {
				Amara::WorldEntity* obj1 = (Amara::WorldEntity*) object1;
				Amara::WorldEntity* obj2 = (Amara::WorldEntity*) object2;

				return obj1->z - obj2->z;
			}
	};
	Amara::ZComparator* zComparator = new Amara::ZComparator();

	/*
	 * Holds a texture/surface/Sound/Music.
	 */
	class Asset {
		public:
			int type;
			void* item;

			Asset(int newType, void* newItem) {
				type = newType;
				item = newItem;
			}
	};


	/*
	 * Holds frame width and height.
	 */
	class Spritesheet: public Amara::Asset {
		public:
			int framewidth = 0;
			int frameheight = 0;

			Spritesheet(SDL_Texture* newtexture, int newwidth, int newheight): Amara::Asset(5, newtexture) {
				type = 5;
				item = newtexture;
				framewidth = newwidth;
				frameheight = newheight;
			}
	};

	/*
	 * Manages the loading of game assets.
	 */
	class Loader {
		public:
			bool destroyed = false;

			Loader(Amara::GameProperties* newProperties) {
				gProperties = newProperties;
				gSurface = gProperties->gSurface;
				gRenderer = gProperties->gRenderer;

				assets = new Amara::LinkedHashMap(100);
			}

			/*
			 * Slow image.
			 */
			bool surface(char* key, std::string path, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;
				// Final optimized image.
				SDL_Surface* optimizedSurface = NULL;

				//Load image at specified path.
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Convert surface to screen format.
					optimizedSurface = SDL_ConvertSurface(loadedSurface, gSurface->format, NULL);

					if (optimizedSurface == NULL) {
						printf("Unable to optimize image %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
						success = false;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(0, optimizedSurface);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool surface(char* key, std::string path) {
				return surface(key, path, false);
			}

			/*
			 * Fast texture image.
			 */
			bool texture(char* key, std::string path, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL && !replace) {
					printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(1, newTexture);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool texture(char* key, std::string path) {
				return texture(key, path, false);
			}

			/*
			 *  Spritesheet handles frame width and height.
			 */
			bool spritesheet(char* key, std::string path, int newwidth, int newheight, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Spritesheet* newAsset = new Amara::Spritesheet(newTexture, newwidth, newheight);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool spritesheet(char* key, std::string path, int newwidth, int newheight) {
				return spritesheet(key, path, newwidth, newheight, false);
			}

			/*
			 * Loads a TrueTypeFont.
			 */
			bool ttf(char* key, std::string path, int size, SDL_Color color, int style, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				// Creating the font
				FC_Font* gFont = FC_CreateFont();
				FC_LoadFont(gFont, gRenderer, path.c_str(), size, color, style);

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(2, gFont);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool ttf(char* key, std::string path, int size, SDL_Color color, int style) {
				return ttf(key, path, size, color, style, false);
			}

			bool ttf(char* key, std::string path, int size, SDL_Color color) {
				return ttf(key, path, size, color, TTF_STYLE_NORMAL);
			}

			bool ttf(char* key, std::string path, int size) {
				return ttf(key, path, size, FC_MakeColor(0,0,0,255));
			}

			bool sfx(char* key, char* path, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				Mix_Chunk* sound = Mix_LoadWAV(path);
				if (sound == NULL) {
					printf("Failed to load sound effect. SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				else {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(3, sound);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool sfx(char* key, char* path) {
				return sfx(key, path, false);
			}

			bool music(char* key, char* path, bool replace) {
				if (get(key) != NULL && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				Mix_Music* music = Mix_LoadMUS(path);
				if (music == NULL) {
					printf("Failed to load music. SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				else {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(3, music);
					assets->add(key, newAsset, replace);
				}

				return success;
			}

			bool music(char* key, char* path) {
				return music(key, path);
			}

			Amara::Asset* get(char* key) {
				return ((Amara::Asset*) assets->get(key));
			}

			void unload(char* key) {
				void* item = assets->remove(key);
				delete item;
			}

			void unload() {
				assets->deepDestroy();
				delete assets;
				assets = new Amara::LinkedHashMap(100);
			}

			void destroy() {
				if (destroyed) return;
				if (assets != NULL) unload();
				assets->deepDestroy();
				delete assets;
				destroyed = true;
			}

		private:
			Amara::GameProperties* gProperties = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;
			Amara::LinkedHashMap* assets = NULL;
	};

	/*
	 * Holds shared data between music and sfx.
	 */
	class Sound: public Amara::WorldEntity {
		public:
			float volume = 1;
			float masterVolume = 1;

			bool fadeOut(float speed, float target) {
				if (action()) {
					if (volume > target) {
						volume -= speed;
					}
					else {
						volume = target;
						nextAction();
					}
					return true;
				}
				return false;
			}

			bool fadeOut(float speed) {
				return fadeOut(speed, 0);
			}

			bool fadeIn(float speed, float target) {
				if (action()) {
					if (volume < target) {
						volume += speed;
					}
					else {
						volume = target;
						nextAction();
					}
					return true;
				}
				return false;
			}

			bool fadeIn(float speed) {
				return fadeIn(speed, 1);
			}

	};

	/*
	 * Holds and handles a sound effect.
	 */
	class SFX: public Amara::Sound {
		public:
			char* key = NULL;
			Mix_Chunk* sfx = NULL;
			int channel = -1;

			SFX(char* newkey) {
				key = newkey;
				inheritEntities = false;
			}

			virtual void play(int loops) {
				if (sfx == NULL) {
					makeSound();
				}
				channel = Mix_PlayChannel(-1, sfx, loops);
				if (channel == -1) {
					printf("Error playing sound: %s\n", key);
					return;
				}
				Mix_Volume(channel, floor(volume * masterVolume * 128));
				paused = false;
			}

			virtual void play() {
				play(0);
			}

			void makeSound() {
				if (load != NULL) {
					Amara::Asset* asset = load->get(key);
					sfx = (Mix_Chunk*) asset->item;
				}
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (sfx == NULL) {
					makeSound();
				}
				else if (channel != -1 && Mix_Playing(channel)) {
					Mix_Volume(channel, floor(volume * masterVolume * 128));
				}
				else {
					channel = -1;
				}
			}
	};

	/*
	 * Holds and handles a piece of music.
	 */
	class Music: public Amara::Sound {
		public:
			char* key = NULL;
			Mix_Music* music = NULL;
			int channel = -1;

			Music(char* newkey) {
				key = newkey;
			}

			void play(int loops) {
				if (music == NULL) {
					makeMusic();
				}
				if (music == NULL) {
					printf("Unable to play sound: %s\n", key);
					return;
				}
				if (Mix_PlayingMusic()) {
					if (game->music == this) {
						if (Mix_PausedMusic()) {
							Mix_ResumeMusic();
						}
					}
					else {
						Mix_HaltMusic();
					}
				}
				Mix_PlayMusic(music, loops);
				game->music = this;

				Mix_VolumeMusic(floor(volume * masterVolume * 128));
				paused = false;
			}

			void play() {
				play(0);
			}

			void pause() {
				if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
					if (game->music == this) {
						Mix_PauseMusic();
						paused = true;
					}
				}
			}

			void resume() {
				if (Mix_PlayingMusic() && Mix_PausedMusic()) {
					if (game->music == this) {
						Mix_ResumeMusic();
						paused = false;
					}
				}
			}

			bool finished() {

			}

			void stop() {
				if (Mix_PlayingMusic()) {
					if (game->music == this) {
						Mix_HaltMusic();
						paused = true;
					}
				}
			}

			void makeMusic() {
				if (load != NULL) {
					Amara::Asset* asset = load->get(key);
					music = (Mix_Music*) asset->item;
				}
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (Mix_PlayingMusic() && game->music == this) {
					if (volume < 0) {
						volume = 0;
					}
					if (volume > 1) {
						volume = 1;
					}

					Mix_VolumeMusic(floor(volume * masterVolume * 128));
				}
			}

			void destroy() {
				if (destroyed) {
					return;
				}
				Amara::WorldEntity::destroy();
				delete key;
			}
	};

	/*
	 * Draws a texture to the screen without the complexities of a Sprite.
	 */
	class Image: public Amara::WorldEntity {
		public:
			Amara::Asset* asset = NULL;
			char* assetName = NULL;
			SDL_Texture* drawTexture = NULL;

			int fullimgwidth = 0;
			int fullimgheight = 0;

			Image(int newx, int newy) {
				x = newx;
				y = newy;

				scale = new Amara::floatVector(1, 1);
				anchor = new Amara::floatVector(0, 0);
			}

			Image(int newx, int newy, char* key): Image(newx, newy) {
				assetName = key;
			}

			Image(int newx, int newy, SDL_Texture* newTexture): Image(newx, newy) {
				setTexture(newTexture);
			}

			virtual bool setTexture(SDL_Texture* newTexture) {
				if (newTexture == NULL) return false;
				drawTexture = newTexture;
				SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
				width = fullimgwidth;
				height = fullimgheight;
				return true;
			}

			virtual bool setTexture(char* key) {
				if (key == NULL || load == NULL) return false;
				assetName = key;
				asset = load->get(key);
				return setTexture(asset);
			}

			virtual bool setTexture(char* key, Amara::Loader* newLoader) {
				load = newLoader;
				return setTexture(key);
			}

			virtual bool setTexture(Amara::Asset* newAsset) {
				asset = newAsset;
				if (asset == NULL) {
					return false;
				}
				Amara::Spritesheet* spritesheet = NULL;
				switch (asset->type) {
					case 0:
						drawTexture = SDL_CreateTextureFromSurface(game->gRenderer, (SDL_Surface*) asset->item);
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = fullimgwidth;
						height = fullimgheight;
						break;
					case 1:
						drawTexture = (SDL_Texture*)asset->item;
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = fullimgwidth;
						height = fullimgheight;
						break;
					case 5:
						drawTexture = (SDL_Texture*)asset->item;
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = fullimgwidth;
						height = fullimgheight;
					default:
						asset = NULL;
						return false;
						break;
				}
				return true;
			}

			virtual void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();
				culled = false;

				runEntities();
			}

			virtual void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (drawTexture == NULL) {
					if (assetName == NULL) return;
					if (!setTexture(assetName)) {
						printf("Image Error: Could not retrieve texture %s.\n", assetName);
						return;
					}
				}

				if (destroyed || scale->x == 0 || scale->y == 0) {
					return;
				}
				if (fullimgwidth <= 0 || fullimgheight <= 0 || parentwidth <= 0 || parentheight <= 0) {
					return;
				}

				if ((x - game->camera->x - anchor->x*fullimgwidth) * game->camScale->x > parentwidth) {
					culled = true;
					return;
				}
				if ((x - game->camera->x - anchor->x*fullimgwidth*scale->x + fullimgwidth * scale->x) * game->camScale->x < 0) {
					culled = true;
					return;
				}
				if ((y - game->camera->y - anchor->y*fullimgheight) * game->camScale->y > parentheight) {
					culled = true;
					return;
				}
				if ((y - game->camera->y - anchor->y*fullimgheight*scale->x + fullimgheight * scale->y) * game->camScale->y < 0) {
					culled = true;
					return;
				}

				if (!(shown && alpha > 0)) {
					return;
				}

				// crop rect
				SDL_Rect* cropRect = new SDL_Rect;

				cropRect->x = 0;
				cropRect->y = 0;
				cropRect->w = floor(fullimgwidth);
				cropRect->h = floor(fullimgheight);
				if (floor((floor(x) - game->camera->x - anchor->x*fullimgwidth*scale->x) * game->camScale->x) < 0) {
					cropRect->x += floor(-((floor(x) - game->camera->x - anchor->x*fullimgwidth*scale->x) * game->camScale->x) / (scale->x * game->camScale->x));
					cropRect->w -= floor(-((floor(x) - game->camera->x - anchor->x*fullimgwidth*scale->x) * game->camScale->x) / (scale->x * game->camScale->x));
				}
				if (floor((floor(y) - game->camera->y - anchor->y*fullimgheight*scale->y) * game->camScale->y) < 0) {
					cropRect->y += floor(-((floor(y) - game->camera->y - anchor->y*fullimgheight*scale->y) * game->camScale->y) / (scale->y * game->camScale->y));
					cropRect->h -= floor(-((floor(y) - game->camera->y - anchor->y*fullimgheight*scale->y) * game->camScale->y) / (scale->y * game->camScale->y));
				}



				Amara::floatVector* newPos = new Amara::floatVector((x - game->camera->x - anchor->x*fullimgwidth*scale->x) * game->camScale->x, (y - game->camera->y - anchor->y*fullimgheight*scale->y) * game->camScale->y);
				if (newPos->x < 0) {
					newPos->x = 0;
				}
				if (newPos->y < 0) {
					newPos->y = 0;
				}

				if (floor(floor(newPos->x) + cropRect->w * scale->x * game->camScale->x)  > parentwidth) {
					cropRect->w -= floor((((floor(newPos->x)) + cropRect->w * scale->x * game->camScale->x) - parentwidth) / (scale->x * game->camScale->x));
				}
				if (floor(floor(newPos->y) + cropRect->h * scale->y * game->camScale->y) > parentheight) {
					cropRect->h -= floor((((floor(newPos->y)) + cropRect->h * scale->y * game->camScale->y) - parentheight) / (scale->y * game->camScale->y));
				}

				//destination rectangle
				SDL_Rect* destRect = new SDL_Rect;

				destRect->x = floor(parentx + newPos->x);
				destRect->y = floor(parenty + newPos->y);
				destRect->w = ceil(cropRect->w * scale->x * game->camScale->x);
				destRect->h = ceil(cropRect->h * scale->y * game->camScale->y);
				if (destRect->x < parentx) {
					destRect->x = parentx;
				}
				if (destRect->y < parenty) {
					destRect->y = parenty;
				}

				if (drawRect == NULL) {
					drawRect = new Amara::intRectangle();
				}
				drawRect->x = destRect->x;
				drawRect->y = destRect->y;
				drawRect->width = destRect->w;
				drawRect->height = destRect->h;

				SDL_SetTextureBlendMode(drawTexture, blendMode);
				SDL_SetTextureAlphaMod(drawTexture, alpha * 255);
				SDL_RenderCopy(game->gRenderer, drawTexture, cropRect, destRect);

				delete cropRect;
				delete destRect;
				delete newPos;
			}

			void destroy() {
				if (destroyed) {
					return;
				}

				if (asset != NULL && asset->type == 0) {
					delete drawTexture;
				}

				Amara::WorldEntity::destroy();
			}
	};

	/*
	 * Holds the frame information for the animation of a sprite.
	 */
	class Animation {
		public:
			int* frames = NULL;
			float speed = 0;
			float defaultSpeed = 0;
			bool looped = false;

			int currentFrame = 0;
			int frameCounter = 0;
			int frameIterator = 0;

			bool locked = false;

			bool destroyed = false;

			bool paused = false;

			Animation(int* newframes, float newspeed, bool newloop) {
				frames = newframes;
				speed = newspeed;
				defaultSpeed = newspeed;
				looped = newloop;

				reset();
			}

			Animation(int* newframes, float newspeed, bool newloop, bool newlocked): Animation(newframes, newspeed, newloop) {
				locked = newlocked;
			}

			void lock() {
				locked = true;
			}

			void unlock() {
				locked = false;
			}

			void pause() {
				paused = true;
			}

			void resume() {
				paused = false;
			}

			int run(bool unlocker) {
				if ((locked || paused) && !unlocker) return currentFrame;
				if (speed < 0) speed = 0;
				frameCounter += 1;
				if (frameCounter >= speed) {
					frameCounter = 0;
					frameIterator += 1;

					if (frames[frameIterator] == -1) {
						if (looped) {
							reset();
						}
						else {
							frameIterator -= 1;
						}
					}
					else {
						currentFrame = frames[frameIterator];
					}
				}

				return currentFrame;
			}

			virtual int run() {
				run(false);
			}

			virtual int getFrame(int i) {
				return frames[i];
			}

			virtual void reset() {
				if (frames == NULL) return;
				speed = defaultSpeed;
				frameCounter = 0;
				frameIterator = 0;
				currentFrame = frames[0];
			}

			void destroy() {
				if (destroyed) return;
				free(frames);
			}
	};

	/*
	 * Costume is a still one frame animation. People may prefer this to setting the frame.
	 */
	class Costume: public Amara::Animation {
		public:
			Costume(int newframe): Amara::Animation(NULL, 0, false, true) {
				currentFrame = newframe;
			}

			int run() {
				return currentFrame;
			}

			void reset() {
				return;
			}
	};

	/*
	 * Sprites drawn onto the screen.
	 */
	class Sprite: public Amara::Image {
		public:
			int framewidth = 0;
			int frameheight = 0;

			int frame = 0;

			char* currentAnimationName = NULL;
			Amara::Animation* currentAnimation = NULL;
			Amara::LinkedHashMap* animations = NULL;
			int aniCounter = 0;

			Sprite(int newx, int newy): Amara::Image(newx, newy) {
				x = newx;
				y = newy;

				scale = new Amara::floatVector(1, 1);
				anchor = new Amara::floatVector(0, 0);

				animations = new Amara::LinkedHashMap(10);
			}

			Sprite(int newx, int newy, char* key): Sprite(newx, newy) {
				assetName = key;
			}

			Sprite(int newx, int newy, char* key, int newframe): Sprite(newx, newy, key) {
				frame = newframe;
			}

			virtual bool setTexture(SDL_Texture* newTexture) {
				drawTexture = newTexture;
				width = fullimgwidth;
				height = fullimgheight;
				framewidth = fullimgwidth;
				frameheight = fullimgheight;
			}

			virtual bool setTexture(char* key) {
				if (key == NULL || load == NULL) return false;
				assetName = key;
				asset = load->get(key);
				return setTexture(asset);
			}

			virtual bool setTexture(char* key, Amara::Loader* newLoader) {
				load = newLoader;
				return setTexture(key);
			}

			bool setTexture(Amara::Asset* newAsset) {
				asset = newAsset;
				if (asset == NULL) {
					return false;
				}
				Amara::Spritesheet* spritesheet = NULL;
				switch (asset->type) {
					case 0:
						drawTexture = SDL_CreateTextureFromSurface(game->gRenderer, (SDL_Surface*) asset->item);
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = fullimgwidth;
						height = fullimgheight;
						framewidth = fullimgwidth;
						frameheight = fullimgheight;
						break;
					case 1:
						drawTexture = (SDL_Texture*)asset->item;
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = fullimgwidth;
						height = fullimgheight;
						framewidth = fullimgwidth;
						frameheight = fullimgheight;
						break;
					case 5:
						spritesheet = (Amara::Spritesheet*) asset;
						drawTexture = (SDL_Texture*)asset->item;
						SDL_QueryTexture(drawTexture, NULL, NULL, &fullimgwidth, &fullimgheight);
						width = spritesheet->framewidth;
						height = spritesheet->frameheight;
						framewidth = spritesheet->framewidth;
						frameheight = spritesheet->frameheight;
						break;
					default:
						asset = NULL;
						drawTexture = NULL;
						return false;
						break;
				}
				return true;
			}

			void add(char* newname, Amara::Animation* newani) {
				animations->add(newname, newani);
			}

			virtual Amara::Animation* play(char* key) {
				Amara::Animation* ani = (Amara::Animation*)animations->get(key);
				if (ani != NULL) {
					if (ani != currentAnimation) {
						ani->reset();
						currentAnimationName = key;
					}
					currentAnimation = ani;
				}
				return ani;
			}

			virtual Amara::Animation* play(Amara::Animation* ani) {
				if (ani != NULL) {
					if (ani != currentAnimation) ani->reset();
					currentAnimation = ani;
				}
				return ani;
			}

			Amara::Animation* getAnimation(char* key) {
				if (key == NULL || animations == NULL) return NULL;
				return (Amara::Animation*)animations->get(key);
			}

			void setAnimationSpeed(float newspeed) {
				if (currentAnimation == NULL) return;
				currentAnimation->speed = newspeed;
			}

			void resetAnimationSpeed() {
				if (currentAnimation == NULL) return;
				currentAnimation->speed = currentAnimation->defaultSpeed;
			}

			void changeAnimationSpeed(float change) {
				if (currentAnimation == NULL) return;
				currentAnimation->speed += change;
			}

			float getAnimationSpeed() {
				if (currentAnimation == NULL) return -1;
				return currentAnimation->speed;
			}

			virtual void create() {

			}

			virtual void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();
				culled = false;

				if (currentAnimation != NULL) {
					frame = currentAnimation->run();
				}

				runEntities();
			}

			virtual void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (drawTexture == NULL) {
					if (assetName == NULL) return;
					if (!setTexture(assetName)) {
						printf("Sprite Error: Could not retrieve texture %s.\n", assetName);
						return;
					}
				}

				if (destroyed || framewidth <= 0 || frameheight <= 0 || scale->x == 0 || scale->y == 0) {
					return;
				}
				if (fullimgwidth <= 0 || fullimgheight <= 0 || parentwidth <= 0 || parentheight <= 0) {
					return;
				}

				if ((x - game->camera->x - anchor->x*framewidth*scale->x) * game->camScale->x > parentwidth) {
					culled = true;
					return;
				}
				if ((x - game->camera->x - anchor->x*framewidth*scale->x + framewidth * scale->x) * game->camScale->x < 0) {
					culled = true;
					return;
				}
				if ((y - game->camera->y - anchor->y*frameheight*scale->y) * game->camScale->y > parentheight) {
					culled = true;
					return;
				}
				if ((y - game->camera->y - anchor->y*frameheight*scale->y + frameheight * scale->y) * game->camScale->y < 0) {
					culled = true;
					return;
				}

				if (!(shown && alpha > 0)) {
					return;
				}
				SDL_Texture* newTexture = NULL;

				// Check frame bounds
				if (frame > (fullimgwidth/framewidth) * (fullimgheight/frameheight) - 1) {
					frame = 0;
				}

				// crop rect
				SDL_Rect* cropRect = new SDL_Rect;

				cropRect->x = floor((frame % (fullimgwidth/framewidth)) * framewidth);
				cropRect->y = floor((frame / (fullimgwidth/framewidth)) * frameheight);
				cropRect->w = floor(framewidth);
				cropRect->h = floor(frameheight);
				if (floor((floor(x) - game->camera->x - anchor->x*framewidth*scale->x) * game->camScale->x) < 0) {
					cropRect->x += floor(-((floor(x) - game->camera->x - anchor->x*framewidth*scale->x) * game->camScale->x) / (scale->x * game->camScale->x));
					cropRect->w -= floor(-((floor(x) - game->camera->x - anchor->x*framewidth*scale->y) * game->camScale->x) / (scale->x * game->camScale->x));
				}
				if (floor((floor(y) - game->camera->y - anchor->y*frameheight*scale->y) * game->camScale->y) < 0) {
					cropRect->y += floor(-((floor(y) - game->camera->y - anchor->y*frameheight*scale->x) * game->camScale->y) / (scale->y * game->camScale->y));
					cropRect->h -= floor(-((floor(y) - game->camera->y - anchor->y*frameheight*scale->y) * game->camScale->y) / (scale->y * game->camScale->y));
				}

				Amara::floatVector* newPos = new Amara::floatVector((x - game->camera->x - anchor->x*framewidth*scale->x) * game->camScale->x, (y - game->camera->y - anchor->y*frameheight*scale->y) * game->camScale->y);
				if (newPos->x < 0) {
					newPos->x = 0;
				}
				if (newPos->y < 0) {
					newPos->y = 0;
				}

				if (floor(floor(newPos->x) + cropRect->w * scale->x * game->camScale->x)  > parentwidth) {
					cropRect->w -= floor((((floor(newPos->x)) + cropRect->w * scale->x * game->camScale->x) - parentwidth) / (scale->x * game->camScale->x));
				}
				if (floor(floor(newPos->y) + cropRect->h * scale->y * game->camScale->y) > parentheight) {
					cropRect->h -= floor((((floor(newPos->y)) + cropRect->h * scale->y * game->camScale->y) - parentheight) / (scale->y * game->camScale->y));
				}

				//destination rectangle
				SDL_Rect* destRect = new SDL_Rect;

				destRect->x = floor(parentx + newPos->x);
				destRect->y = floor(parenty + newPos->y);
				destRect->w = ceil(cropRect->w * scale->x * game->camScale->x);
				destRect->h = ceil(cropRect->h * scale->y * game->camScale->y);
				if (destRect->x < parentx) {
					destRect->x = parentx;
				}
				if (destRect->y < parenty) {
					destRect->y = parenty;
				}

				if (drawRect == NULL) {
					drawRect = new Amara::intRectangle();
				}
				drawRect->x = destRect->x;
				drawRect->y = destRect->y;
				drawRect->width = destRect->w;
				drawRect->height = destRect->h;

				SDL_SetTextureBlendMode(drawTexture, blendMode);
				SDL_SetTextureAlphaMod(drawTexture, alpha * 255);
				SDL_RenderCopy(game->gRenderer, drawTexture, cropRect, destRect);

				delete cropRect;
				delete destRect;
				delete newPos;
			}

			void destroy() {
				if (destroyed) {
					return;
				}
				destroyAnimations();
				if (asset != NULL && asset->type == 0) {
					delete drawTexture;
				}

				Amara::WorldEntity::destroy();
			}

			void destroyAnimations() {
				if (animations == NULL) return;
				Amara::LinkedItem* curItem = animations->getStart();
				Amara::LinkedItem* nextItem = NULL;
				Amara::Animation* curAni = NULL;

				while (curItem != NULL) {
					curAni = (Amara::Animation*)curItem->item;
					if (!curAni->locked) {
						curAni->destroy();
						delete curAni;
					}

					nextItem = curItem->next;
					curItem = nextItem;
				}
				animations->destroy();
				delete animations;
				currentAnimation = NULL;
				animations = NULL;
			}
	};

	/*
	 * Used to create and manage sprites in a TileMap.
	 * Added functionality for path finding.
	 */
	class Tile: public Amara::Sprite {
		public:
			float setAlpha = 1;
			Amara::intVector* pos = NULL;
			int value = -1;

			bool isWall = false;
			bool walkable = true;

			int order[8];
			int orderCount = 0;

			Tile* north = NULL;
			Tile* east = NULL;
			Tile* south = NULL;
			Tile* west = NULL;
			Tile* northeast = NULL;
			Tile* northwest = NULL;
			Tile* southeast = NULL;
			Tile* southwest = NULL;

			Tile* lastTile = NULL;
			Tile* nextTile = NULL;

			bool open = false;

			Tile* recTargetTile = NULL;
			int recTargetDistance = -1;

			Tile* recStartTile = NULL;
			int recStartDistance = -1;

			int gcost = 0;
			int hcost = 0;
			int fcost = 0;

			Tile(int newx, int newy): Amara::Sprite(newx, newy) {
				pos = new Amara::intVector(newx, newy);
			}

			void resetPath(Tile* start, Tile* target) {
				nextTile = NULL;
				lastTile = NULL;

				recStartTile = start;
				recStartDistance = -1;

				recTargetTile = target;
				recTargetDistance = -1;
			}

			void calculateCosts(int newgcost) {
				gcost = newgcost;
				hcost = distanceToTile(recTargetTile);

				fcost = gcost + hcost;
			}

			void calculateCosts() {
				calculateCosts(distanceToTile(recStartTile));
			}

			int distanceToTile(Amara::Tile* target) {
				int recDistance = 0;

				if (target == recTargetTile && recTargetDistance != -1) {
					return recTargetDistance;
				}

				if (target == recStartTile && recStartDistance != -1) {
					return recStartDistance;
				}

				if (pos->x == target->pos->x && pos->y == target->pos->y) {
					recDistance = 0;
					return 0;
				}

				if (pos->x < target->pos->x) {
					if (pos->y < target->pos->y) {
						recDistance = 1 + southeast->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}
					else if (pos->y > target->pos->y) {
						recDistance = 1 + northeast->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}

					recDistance = 1 + east->distanceToTile(target);
					if (target == recTargetTile) {
						recTargetDistance = recDistance;
					}
					else if (target == recStartTile) {
						recStartDistance = recDistance;
					}
					return recDistance;
				}
				else if (pos->x > target->pos->x) {
					if (pos->y < target->pos->y) {
						recDistance = 1 + southwest->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}
					else if (pos->y > target->pos->y) {
						recDistance = 1 + northwest->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}

					recDistance = 1 + west->distanceToTile(target);
					if (target == recTargetTile) {
						recTargetDistance = recDistance;
					}
					else if (target == recStartTile) {
						recStartDistance = recDistance;
					}
					return recDistance;
				}
				else {
					if (pos->y < target->pos->y) {
						recDistance = 1 + south->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}
					else if (pos->y > target->pos->y) {
						recDistance = 1 + north->distanceToTile(target);
						if (target == recTargetTile) {
							recTargetDistance = recDistance;
						}
						else if (target == recStartTile) {
							recStartDistance = recDistance;
						}
						return recDistance;
					}
				}
			}

			bool aroundCorner(Amara::Tile* other) {
				if (other == NULL) return false;

				if (other == northeast) {
					if (north == NULL || north->isWall) {
						return true;
					}
					if (east == NULL || east->isWall) {
						return true;
					}
				}
				else if (other == southeast) {
					if (east == NULL || east->isWall) {
						return true;
					}
					if (south == NULL || south->isWall) {
						return true;
					}
				}
				else if (other == southwest) {
					if (south == NULL || south->isWall) {
						return true;
					}
					if (west == NULL || west->isWall) {
						return true;
					}
				}
				else if (other == northwest) {
					if (west == NULL || west->isWall) {
						return true;
					}
					if (north == NULL || north->isWall) {
						return true;
					}
				}

				return false;
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				x = pos->x * width + parent->x;
				y = pos->y * height + parent->y;
				if (asset != NULL) {
					Amara::Sprite::draw(parentx, parenty, parentwidth, parentheight);
				}
			}
	};

	/*
	 * Handles a 2d grid of tiles.
	 */
	class TileMap: public Amara::WorldEntity {
		public:
			int** map = NULL;
			Amara::Tile*** tiles = NULL;
			Amara::intVector* size = NULL;

			bool makingPath = false;
			Tile* path = NULL;
			Tile* startTile = NULL;
			Tile* targetTile = NULL;

			char* texture = NULL;

			bool walkDiagonally = false;
			bool throughCorners = false;

			int walls[100];
			bool emptyIsWall = true;

			TileMap(int sizex, int sizey, char* key) {
				size = new Amara::intVector(sizex, sizey);
				createMap();
				texture = key;
				resetWalls();
			}

			void addWall(int value) {
				for (int i = 0; i < 100; i++) {
					if (walls[i] == -1) {
						walls[i] = value;
						return;
					}
				}
			}

			void addWalls(int newWalls[]) {
				int count = 0;
				while (newWalls[count] != -1 && count < 100) {
					walls[count] = newWalls[count];
					count += 1;
				}
			}

			void resetWalls() {
				for (int i = 0; i < 100; i++) {
					walls[i] = -1;
				}
			}

			bool isWall(int value) {
				if (value == -1) {
					return emptyIsWall;
				}
				for (int i = 0; i < 100; i++) {
					if (walls[i] == -1) {
						return false;
					}
					if (walls[i] == value) {
						return true;
					}
				}
				return false;
			}

			bool isWall(Tile* checkTile) {
				if (isWall(checkTile->value)) {
					checkTile->isWall = true;
					return true;
				}
				else {
					checkTile->isWall = false;
					return false;
				}
			}

			bool isWall(int tilex, int tiley) {
				if (tilex < 0 || tilex > size->x -1 || tiley < 0 || tiley > size->y) {
					return emptyIsWall;
				}
				return isWall(tiles[tilex][tiley]);
			}

			void createMap() {
				if (map != NULL) {
					destroyMap();
				}
				if (tiles != NULL) {
					destroyTiles();
				}

				map = new int*[size->x];
				tiles = new Amara::Tile**[size->x];
				for (int i = 0; i < size->x; i++) {
					map[i] = new int[size->y];
					tiles[i] = new Amara::Tile*[size->y];
					for (int j = 0; j < size->y; j++) {
						map[i][j] = -1;
						tiles[i][j] = new Amara::Tile(i, j);
						tiles[i][j]->game = game;
						tiles[i][j]->load = load;
						tiles[i][j]->parent = this;
						tiles[i][j]->value = -1;

						if (texture != NULL) {
							tiles[i][j]->setTexture(texture);
						}

						if (j > 0) {
							tiles[i][j]->north = tiles[i][j-1];
							tiles[i][j-1]->south = tiles[i][j];

							if (i > 0) {
								tiles[i][j]->northwest = tiles[i-1][j-1];
								tiles[i-1][j-1]->southeast = tiles[i][j];
							}
						}
						if (i > 0) {
							tiles[i][j]->west = tiles[i-1][j];
							tiles[i-1][j]->east = tiles[i][j];

							if (j < size->y - 1) {
								tiles[i][j]->southwest = tiles[i-1][j+1];
								tiles[i-1][j+1]->northeast = tiles[i][j];
							}
						}

						tiles[i][j]->isWall = emptyIsWall;
					}
				}
			}

			Amara::Animation* add(char* key, Amara::Animation* newAni) {
				if (animations == NULL) {
					animations = new Amara::LinkedHashMap(10);
				}
				animations->add(key, newAni);
				return newAni;
			}

			Amara::WorldEntity* add(Amara::WorldEntity* newEntity) {
				return Amara::WorldEntity::add(newEntity);
			}

			void setTexture(char* key) {
				texture = key;
				if (texture != NULL && tiles != NULL) {
					for (int i = 0; i < size->x; i++) {
						for (int j = 0; j < size->y; j++) {
							tiles[i][j]->setTexture(texture);
						}
					}
				}
			}

			void setTile(int setx, int sety, int value) {
				if (map == NULL) return;
				if (map[setx] == NULL) return;
				map[setx][sety] = value;
				if (value >= 0) tiles[setx][sety]->frame = value;
				if (value >= 0 && animations != NULL) {
					Amara::LinkedItem* curItem = animations->getStart();
					while (curItem != NULL) {
						Amara::Animation* ani = (Amara::Animation*) curItem->item;
						tiles[setx][sety]->value = value;
						if (value == ani->getFrame(0)) {
							tiles[setx][sety]->play(ani);
							return;
						}
						isWall(tiles[setx][sety]);
					}
				}
			}

			void setTile(int setx, int sety, char* key) {
				Amara::Animation* ani = (Amara::Animation*)animations->get(key);
				if (ani == NULL) return;
				if (map == NULL) return;
				if (map[setx] == NULL) return;
				map[setx][sety] = ani->getFrame(0);
				tiles[setx][sety]->value = ani->getFrame(0);
				tiles[setx][sety]->play(ani);
				isWall(tiles[setx][sety]);
			}

			int getTileAt(int getx, int gety) {
				if (getx >= size->x || gety >= size->y) {
					printf("Tilemap Error: Arguments for 'Amara::TileMap::getTileAt(int getx, int gety)' is out of map bounds.\n");
					printf("Attempted to get (%d, %d) from map of bounds (%d, %d).\n", getx, gety, size->x, size->y);
					return -1;
				}
				return map[getx][gety];
			}


			void feedMap(int newMap[]) {
				createMap();
				for (int i = 0; i < size->x; i++) {
					for (int j = 0; j < size->y; j++) {
						map[i][j] = newMap[i + j*size->x];
						tiles[i][j]->frame = map[i][j];
						tiles[i][j]->value = map[i][j];
						isWall(tiles[i][j]);
					}
				}
			}

			void feedMap(int newMap[], int setx, int sety) {
				if (size == NULL) {
					size = new intVector(setx, sety);
				}
				else {
					size->x = setx;
					size->y = sety;
				}

				feedMap(newMap);
			}

			void clearMap(int newMap[]) {
				for (int i = 0; i < size->x*size->y; i++) {
					newMap[i] = -1;
				}
			}

			void clearMap(int newMap[], int sizex, int sizey) {
				for (int i = 0; i < sizex*sizey; i++) {
					newMap[i] = -1;
				}
			}

			void findPath(int fromx, int fromy, int tox, int toy) {
				if (makingPath) return;
				if (fromx < 0 || fromx >= size->x || fromy < 0 || fromy >= size->y) return;
				if (tox < 0 || tox >= size->x || toy < 0 || toy >= size->y) return;

				startTile = tiles[fromx][fromy];
				if (startTile != NULL && (startTile->isWall || !startTile->walkable)) {
					return;
				}
				targetTile = tiles[tox][toy];
				if (targetTile != NULL && (targetTile->isWall || !targetTile->walkable)) {
					return;
				}

				path = NULL;
				makingPath = true;

				for (int i = 0; i < size->x; i++) {
					for (int j = 0; j < size->y; j++) {
						tiles[i][j]->resetPath(startTile, targetTile);
					}
				}

				SDL_CreateThread(makePath, "Making Path", this);
			}

			Amara::Tile* nextInPath() {
				path = path->nextTile;

				return path;
			}

			void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();
				runAnimations();
				for (int i = 0; i < size->x; i++) {
					for (int j = 0; j < size->y; j++) {
						tiles[i][j]->run();
					}
				}
				Amara::WorldEntity::runEntities();
			}

			void runAnimations() {
				if (animations == NULL) return;
				Amara::LinkedItem* curItem = animations->getStart();

				while (curItem != NULL) {
					Amara::Animation* curAni = (Amara::Animation*) curItem->item;
					if (curAni != NULL) {
						curAni->unlock();
						curAni->run();
						curAni->lock();
					}
				}
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed || paused) return;
				for (int i = 0; i < size->x; i++) {
					for (int j = 0; j < size->y; j++) {
						if (tiles[i][j]->setAlpha < 0) {
							tiles[i][j]->setAlpha = 0;
						}
						else if (tiles[i][j]->setAlpha > 1) {
							tiles[i][j]->setAlpha = 1;
						}

						tiles[i][j]->alpha = alpha * tiles[i][j]->setAlpha;
						tiles[i][j]->blendMode = blendMode;
						tiles[i][j]->draw(parentx, parenty, parentwidth, parentheight);
					}
				}
				Amara::WorldEntity::drawEntities(parentx, parenty, parentwidth, parentheight);
			}

			void destroyMap() {
				if (map == NULL) return;
				for (int i = 0; i < size->x; i++) {
					delete[] map[i];
				}
				delete[] map;
			}

			void destroyTiles() {
				if (tiles == NULL) return;
				for (int i = 0; i < size->x; i++) {
					for (int j = 0; j < size->y; j++) {
						tiles[i][j]->destroy();
					}
					delete[] tiles[i];
				}
				delete[] tiles;
			}

			void destroy() {
				if (destroyed) return;
				destroyMap();
				destroyTiles();
				if (animations != NULL) animations->deepDestroy();
				Amara::WorldEntity::destroy();
				delete size;
			}

		private:
			Amara::LinkedHashMap* animations = NULL;
	};

	/*
	 * Gets the tile with the smalled fcost in a list.
	 */
	Amara::Tile* getSmallestFCost(Amara::LinkedList* list) {
		if (list == NULL) return NULL;

		Amara::LinkedItem* curItem = list->startItem;
		if (curItem == NULL) {
			return NULL;
		}

		Amara::Tile* smallest = NULL;

		while (curItem != NULL) {
			Amara::Tile* curTile = (Amara::Tile*) curItem->item;
			if (smallest == NULL) {
				smallest = curTile;
			}

			if (curTile->fcost < smallest->fcost) {
				smallest = curTile;
			}
			else if (curTile->fcost == smallest->fcost) {
				if (curTile->hcost <  smallest->hcost) {
					smallest = curTile;
				}
			}

			curItem = curItem->next;
		}

		return smallest;
	}

	void checkNeighbor(Amara::LinkedList* openList, Amara::LinkedList* closedList, Amara::Tile* current, Amara::Tile* neighbor, int weight) {
		if (neighbor != NULL && !current->aroundCorner(neighbor)) {
			if (!neighbor->isWall && neighbor->walkable && !closedList->contains(neighbor)) {
				if (current->gcost + weight < neighbor->gcost || !openList->contains(neighbor)) {
					neighbor->calculateCosts(current->gcost + weight);
					neighbor->lastTile = current;
					if (!openList->contains(neighbor)) {
						openList->push(neighbor);
					}
				}
			}
		}
	}

	/*
	 * Used to find the path from one tile to another via multithreading.
	 */
	int makePath(void* data) {
		Amara::TileMap* map = (Amara::TileMap*) data;

		Amara::LinkedList* openList = new Amara::LinkedList();
		Amara::LinkedList* closedList = new Amara::LinkedList();

		openList->push(map->startTile);
		map->startTile->calculateCosts();

		bool findingPath = true;

		Amara::Tile* current = NULL;
		Amara::Tile* neighbor = NULL;

		bool pathFound = false;

		while (findingPath) {
			if (openList->length() == 0) {
				findingPath = false;
				break;
			}

			current = getSmallestFCost(openList);
			openList->remove(current);
			closedList->push(current);

			if (current == NULL) {
				map->path = NULL;
				findingPath = false;
			}
			else if (current == map->targetTile) {
				findingPath = false;
				pathFound = true;
			}
			else {
				checkNeighbor(openList, closedList, current, current->north, 10);
				checkNeighbor(openList, closedList, current, current->east, 10);
				checkNeighbor(openList, closedList, current, current->south, 10);
				checkNeighbor(openList, closedList, current, current->west, 10);
				if (map->walkDiagonally) {
					if (current->pos->y > 0 && current->pos->x < map->size->x-1) {
						checkNeighbor(openList, closedList, current, current->northeast, 14);
					}
					if (current->pos->y < map->size->y-1 && current->pos->x < map->size->x-1) {
						checkNeighbor(openList, closedList, current, current->southeast, 14);
					}
					if (current->pos->y > 0 && current->pos->x > 0) {
						checkNeighbor(openList, closedList, current, current->northwest, 14);
					}
					if (current->pos->y < map->size->y-1 && current->pos->x > 0) {
						checkNeighbor(openList, closedList, current, current->southwest, 14);
					}
				}
			}
		}

		if (pathFound) {
			current = map->targetTile;
			while (current != map->startTile) {
				neighbor = current->lastTile;
				neighbor->nextTile = current;
				current = neighbor;
			}
			map->path = map->startTile->nextTile;
			map->makingPath = false;
		}

		openList->destroy();
		closedList->destroy();
		delete openList;
		delete closedList;
		return 0;
	}

	/*
	 * Renders text to the screen using a True Type Font.
	 */
	class Text: public Amara::WorldEntity {
		public:
			char* font = NULL;
			char* text = NULL;
			SDL_Color textColor;
			FC_AlignEnum alignment = FC_ALIGN_LEFT;

			int length = 0;

			Text(int startx, int starty, char* newfont, char* newtext) {
				x = startx;
				y = starty;

				font = newfont;
				text = newtext;

				if (text != NULL) length = strlen(text);

				scale = new Amara::floatVector(1, 1);
			}

			Text(int startx, int starty, char* newfont):
			Text(startx, starty, newfont, NULL) {}

			void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();

				runEntities();
			}

			void setText(char* newText) {
				text = newText;
				length = strlen(text);
			}

			void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				textColor.r = r;
				textColor.g = g;
				textColor.b = b;
				textColor.a = a;
			}

			void setColor(Uint8 r, Uint8 g, Uint8 b) {
				setColor(r, g, b, 255);
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed || scale->x == 0 || scale->y == 0) {
					return;
				}
				if (parentwidth <= 0 || parentheight <= 0) {
					return;
				}

				FC_Effect effect = FC_MakeEffect(alignment, FC_MakeScale(scale->x*game->camScale->x, scale->y*game->camScale->y), textColor);

				int drawwidth = parentwidth;
				int drawheight = parentheight;

				Amara::intVector* newPos = new Amara::intVector(x - game->camera->x*game->camScale->x, y - game->camera->y*game->camScale->y);

				if (newPos->x + drawwidth > parentwidth) {
					drawwidth -= (newPos->x + drawwidth) - parentwidth;
				}
				if (newPos->y + drawheight > parentheight) {
					drawheight -= (newPos->y + drawheight) - parentheight;
				}

				FC_Rect box = FC_MakeRect(newPos->x + parentx, newPos->y + parenty, drawwidth, drawheight);

				FC_Font* drawFont = (FC_Font*)(load->get(font))->item;
				FC_DrawBoxEffect(drawFont, game->gRenderer, box, effect, text);

				Amara::WorldEntity::drawEntities(parentx, parenty, parentwidth, parentheight);
			}

			void destroy() {
				if (destroyed) {
					return;
				}
				Amara::WorldEntity::destroy();
				delete font;
				delete text;
			}
	};

	/*
	 * To be used as a drawing canvas.
	 * Sets the rendering bounds for all objects contained within.
	 */
	class Container: public Amara::WorldEntity {
		public:
			bool autoResize = true;

			Container() {
				scale = new Amara::floatVector(1, 1);
				anchor = new Amara::floatVector(0, 0);
			}

			Container(int startX, int startY, int startWidth, int startHeight): Container() {
				x = startX;
				y = startY;
				width = startWidth;
				height = startHeight;
				autoResize = false;
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed || entities == NULL) return;
				if (autoResize) {
					x = 0;
					y = 0;
					width = parentwidth;
					height = parentheight;
				}
				Amara::LinkedItem* curItem = entities->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				int newparentx = floor(x) - game->camera->x;
				int newparenty = floor(y) - game->camera->y;
				int newparentwidth = width * game->camScale->x;
				int newparentheight = height * game->camScale->y;

				game->camera->x = 0;
				game->camera->y = 0;

				if (newparentx < 0) {
					game->camera->x -= newparentx;
					newparentwidth += newparentx * game->camScale->x;
					newparentx = 0;
				}
				if (newparenty < 0) {
					game->camera->y -= newparenty;
					newparentheight += newparenty * game->camScale->y;
					newparenty = 0;
				}

				if (newparentwidth < 0) {
					newparentwidth = 0;
				}
				if (newparentheight < 0) {
					newparentheight = 0;
				}

				if (newparentx + newparentwidth > parentwidth) {
					newparentwidth -= ((newparentx + newparentwidth) - parentwidth);
				}
				if (newparenty + newparentheight > parentheight) {
					newparentheight -= ((newparenty + newparentheight) - parentheight);
				}

				if (newparentwidth < 0) {
					newparentwidth = 0;
				}
				if (newparentheight < 0) {
					newparentheight = 0;
				}

				newparentx += parentx;
				newparenty += parenty;

				while (curItem != NULL) {
					Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;
					if (!curEntity->destroyed) {
						curEntity->draw(newparentx, newparenty, newparentwidth, newparentheight);
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							entities->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}
	};

	/*
	 * Handles drawing to a canvas and applying effects.
	 */
	class Canvas: public Amara::WorldEntity {
		public:
			SDL_Texture* canvas = NULL;
			SDL_Color* recColor = NULL;
			Amara::intVector* recCam = NULL;
			Amara::intVector* recCamScale = NULL;
			float drawAlpha = 1;
			bool updateCanvas = false;
			bool autoResize = false;
			SDL_BlendMode drawBlendMode = SDL_BLENDMODE_BLEND;

			Canvas() {
				autoResize = true;
			}

			Canvas(int newx, int newy, int newwidth, int newheight) {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
			}

			void drawRect(int drawx, int drawy, int drawwidth, int drawheight) {
				if (canvas == NULL || destroyed) return;
				SDL_Rect* rect = new SDL_Rect;
				rect->x = drawx;
				rect->y = drawy;
				rect->w = drawwidth;
				rect->h = drawheight;

				SDL_SetRenderDrawBlendMode(game->gRenderer, drawBlendMode);
				SDL_RenderDrawRect(game->gRenderer, rect);

				delete rect;
			}

			void fillRect(int drawx, int drawy, int drawwidth, int drawheight) {
				if (canvas == NULL || destroyed) return;
				SDL_Rect* rect = new SDL_Rect;
				rect->x = drawx;
				rect->y = drawy;
				rect->w = drawwidth;
				rect->h = drawheight;

				SDL_SetRenderDrawBlendMode(game->gRenderer, drawBlendMode);
				SDL_RenderFillRect(game->gRenderer, rect);

				delete rect;
			}

			void beginFill() {
				if (destroyed) return;

				if (updateCanvas) {
					destroyCanvas();
				}

				if (recCam == NULL) {
					recCam = new intVector(game->camera->x, game->camera->y);
					recCamScale = new intVector(game->camScale->x, game->camScale->y);
				}
				recCam->x = game->camera->x;
				recCam->y = game->camera->y;
				recCamScale->x = game->camScale->x;
				recCamScale->y = game->camScale->y;

				if (canvas == NULL) {
					SDL_SetRenderTarget(game->gRenderer, NULL);
					canvas = SDL_CreateTexture(game->gRenderer, SDL_GetWindowPixelFormat(game->gWindow), SDL_TEXTUREACCESS_TARGET, (int)width, (int)height);
				}

				if (canvas != NULL) {
					SDL_SetRenderTarget(game->gRenderer, canvas);
				}
				else {
					endFill();
				}
				SDL_SetRenderDrawBlendMode(game->gRenderer, SDL_BLENDMODE_BLEND);
			}

			void beginFill(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				beginFill();
				if (canvas == NULL) return;
				recColor = new SDL_Color;
				if (SDL_GetRenderDrawColor(game->gRenderer, &recColor->r, &recColor->g, &recColor->b, &recColor->a) != 0) return;
				if (SDL_SetRenderDrawColor(game->gRenderer, r, g, b, a) != 0) return;
				drawAlpha = a/255;
			}

			void beginFill(Uint8 r, Uint8 g, Uint8 b) {
				beginFill(r, g, b, 255);
			}

			void endFill() {
				if (recCam != NULL) {
					game->camera->x = recCam->x;
					game->camera->y = recCam->y;
					delete recCam;
					recCam = NULL;

					game->camScale->x = recCamScale->x;
					game->camScale->y = recCamScale->y;
					delete recCamScale;
					recCamScale = NULL;
				}
				if (recColor != NULL) {
					SDL_SetRenderDrawColor(game->gRenderer, recColor->r, recColor->g, recColor->b, recColor->a);
					delete recColor;
					recColor = NULL;
				}
				SDL_SetRenderTarget(game->gRenderer, NULL);
				SDL_SetRenderDrawBlendMode(game->gRenderer, SDL_BLENDMODE_NONE);
				drawBlendMode = SDL_BLENDMODE_BLEND;
				drawAlpha = 1;
			}

			void clear() {
				beginFill(0, 0, 0, 0);
				if (canvas == NULL || destroyed) return;

				SDL_Rect* rect = new SDL_Rect;
				rect->x = 0;
				rect->y = 0;
				rect->w = width;
				rect->h = height;

				SDL_SetRenderDrawBlendMode(game->gRenderer, SDL_BLENDMODE_NONE);
				SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_NONE);
				SDL_RenderFillRect(game->gRenderer, rect);

				endFill();
				delete rect;
			}

			void setBlendMode(SDL_BlendMode mode) {
				if (canvas == NULL || destroyed) return;
				SDL_SetRenderDrawBlendMode(game->gRenderer, mode);
				drawBlendMode = mode;
			}

			void draw(Amara::WorldEntity* entity) {
				if (entity == NULL || canvas == NULL || destroyed) return;
				game->camera->x = 0;
				game->camera->y = 0;
				game->camScale->x = 1;
				game->camScale->y = 1;

				entity->blendMode = drawBlendMode;
				entity->game = game;
				entity->load = load;

				entity->draw(0, 0, width, height);
			}

			void draw(int drawx, int drawy, Amara::WorldEntity* entity) {
				if (entity == NULL || destroyed) return;
				entity->x = drawx;
				entity->y = drawy;

				draw(entity);
			}

			void draw(int drawx, int drawy, char* key) {
				if (key == NULL || canvas == NULL || destroyed) return;
				if (load->get(key) == NULL) return;
				game->camera->x = 0;
				game->camera->y = 0;
				game->camScale->x = 1;
				game->camScale->y = 1;

				Amara::Image* newImg = new Amara::Image(drawx, drawy, key);
				newImg->game = game;
				newImg->load = load;
				newImg->alpha = drawAlpha;
				newImg->blendMode = drawBlendMode;

				draw(newImg);

				newImg->destroy();
				delete newImg;
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (autoResize) {
					if (trunc(width) != parentwidth || trunc(height) != parentheight) {
						x = 0;
						y = 0;
						width = parentwidth;
						height = parentheight;
					}
				}
				if (destroyed || canvas == NULL) return;

				SDL_SetRenderTarget(game->gRenderer, canvas);

				SDL_SetRenderTarget(game->gRenderer, NULL);

				Amara::Image* newImg = new Amara::Image(x, y, canvas);
				newImg->game = game;

				newImg->load = load;
				newImg->alpha = alpha;
				newImg->blendMode = blendMode;
				newImg->draw(parentx, parenty, parentwidth, parentheight);
				newImg->destroy();

				delete newImg;
			}

			void destroyCanvas() {
				if (canvas != NULL) {
					SDL_DestroyTexture(canvas);
					canvas = NULL;
				}
			}

			void destroy() {
				if (destroyed) return;
				endFill();
				clear();

				destroyCanvas();

				Amara::WorldEntity::destroy();
			}
	};

	/*
	 * Used to hard control the order of rendering.
	 */
	class Layer: public Amara::WorldEntity {
		public:
			Layer() {

			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				Amara::WorldEntity::drawEntities(parentx, parenty, parentwidth, parentheight);
			}
	};

	class Light: public Amara::Image {
		public:

			Light(int newx, int newy): Amara::Image(newx, newy) {

			}

			Light(int newx, int newy, char* newkey): Amara::Image(newx, newy, newkey) {

			}

			Light(int newx, int newy, float newscalex, float newscaley): Light(newx, newy) {
				scale->x = newscalex;
				scale->y = newscaley;
			}

			Light(int newx, int newy, float newscalex, float newscaley, char* newkey): Light(newx, newy, newkey) {
				scale->x = newscalex;
				scale->y = newscaley;
			}

			Light(int newx, int newy, float newscale, char* newkey): Light(newx, newy, newscale, newscale, newkey) {

			}
	};

	class LightLayer: public Amara::Layer {
		public:
			Amara::Canvas* canvas = NULL;
			Amara::LinkedList* lights = NULL;

			SDL_Color* color = NULL;

			bool autoResize = true;

			LightLayer() {
				canvas = new Amara::Canvas();
				color = new SDL_Color;
			}

			LightLayer(int newx, int newy) {
				x = newx;
				y = newy;
				autoResize = false;

				canvas = new Amara::Canvas();
			}

			LightLayer(int newx, int newy, int newwidth, int newheight) {
				x = newx;
				y = newy;
				autoResize = false;
				width = newwidth;
				height = newheight;

				canvas = new Amara::Canvas(x, y, width, height);
			}

			void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				if (color == NULL) {
					color = new SDL_Color;
				}
				color->r = r;
				color->g = g;
				color->b = b;
				color->a = a;

				if (color->r < 0) color->r = 0;
				if (color->r > 255) color->r = 255;

				if (color->g < 0) color->g = 0;
				if (color->g > 255) color->g = 255;

				if (color->b < 0) color->b = 0;
				if (color->b > 255) color->b = 255;

				if (color->a < 0) color->a = 0;
				if (color->a > 255) color->a = 255;
			}

			void setColor(Uint8 r, Uint8 g, Uint8 b) {
				setColor(r, g, b, 255);
			}

			void modColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				if (color == NULL) {
					color = new SDL_Color;
					color->r = r;
					color->g = g;
					color->b = b;
					color->a = a;
				}
				else {
					color->r += r;
					color->g += g;
					color->b += b;
					color->a += a;
				}

				if (color->r < 0) color->r = 0;
				if (color->r > 255) color->r = 255;

				if (color->g < 0) color->g = 0;
				if (color->g > 255) color->g = 255;

				if (color->b < 0) color->b = 0;
				if (color->b > 255) color->b = 255;

				if (color->a < 0) color->a = 0;
				if (color->a > 255) color->a = 255;
			}

			void modColor(Uint8 r, Uint8 g, Uint8 b) {
				modColor(r, g, b, 0);
			}

			void run() {
				if (!shown || destroyed || canvas == NULL) return;
				Amara::Actor::run();
				runPhysics();
				update();


				canvas->game = game;
				canvas->load = load;
				canvas->clear();
				canvas->alpha = alpha;

				if (color == NULL) {
					canvas->beginFill(0, 0, 0, 255);
				}
				else {
					canvas->beginFill(color->r, color->g, color->b, color->a);
				}
				canvas->setBlendMode(SDL_BLENDMODE_NONE);
				canvas->fillRect(0, 0, width, height);
				canvas->endFill();

				canvas->beginFill();

				if (entities != NULL) {
					Amara::LinkedItem* curItem = entities->startItem;
					Amara::LinkedItem* lastItem = NULL;
					Amara::LinkedItem* nextItem = NULL;

					while (curItem != NULL) {
						Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;
						if (!curEntity->destroyed) {
							canvas->setBlendMode(curEntity->blendMode);
							canvas->draw(curEntity);
							lastItem = curItem;
							curItem = curItem->next;
						}
						else {
							if (lastItem == NULL) {
								nextItem = curItem->next;
								curItem->destroy();
								delete curItem;
								entities->startItem = nextItem;
								curItem = nextItem;
							}
							else {
								lastItem->next = curItem->next;
								curItem->destroy();
								delete curItem;
								curItem = lastItem->next;
							}
						}
					}
				}
				canvas->endFill();

				Amara::Layer::run();
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (autoResize) {
					if (width != parentwidth || height != parentheight) {
						x = 0;
						y = 0;
						width = parentwidth;
						height = parentheight;
					}
				}

				if (!shown || destroyed || canvas == NULL) return;

				canvas->blendMode = SDL_BLENDMODE_MOD;
				canvas->draw(parentx, parenty, parentwidth, parentheight);
			}

			void destroy(bool dlights) {
				if (destroyed) return;
				if (color != NULL) delete color;
				if (canvas != NULL) delete canvas;
				Amara::WorldEntity::destroy();
			}

			void destroy() {
				destroy(true);
			}
	};

	class Generator: public Amara::WorldEntity {
		public:
			int tickCounter = 0;
			int tickRate = 0;

			int tickLife = -1;
			int lifeCounter = 0;

			// Override this. Must make a new entity within, and handle adding it to another entity.
			virtual void generate() = 0;

			void run() {
				if (destroyed || paused) return;
				runPhysics();
				Amara::Actor::run();

				tickCounter += 1;
				if (tickCounter >= tickRate) {
					tickCounter = 0;
					generate();
				}

				if (tickLife > 0) {
					lifeCounter += 1;
					if (lifeCounter > tickLife) {
						destroy();
					}
				}
			}

			void setTickLife(int newlifetime) {
				if (game == NULL) {
					printf("Amara::Generator Error: Please call setTickLife() outside of constructor.");
					Amara::breakGame();
				}
				tickLife = newlifetime;
			}

			void setTimeLife(float newlifetime) {
				if (game == NULL) {
					printf("Amara::Generator Error: Please call setTimeLife() outside of constructor.");
					Amara::breakGame();
				}
				tickLife = newlifetime * game->lps;
			}

			void setTickRate(int newtickrate) {
				if (game == NULL) {
					printf("Amara::Generator Error: Please call setTickRate() outside of constructor.");
					Amara::breakGame();
				}
				tickRate = newtickrate;
			}

			void setTimeRate(float newtimerate) {
				if (game == NULL) {
					printf("Amara::Generator Error: Please call setTimeRate() outside of constructor.");
					Amara::breakGame();
				}
				tickRate = newtimerate * game->lps;
			}
	};

	/*
	 * Used to handle views on the screen.
	 */
	class Camera: public Amara::WorldEntity {
		public:
			Amara::intRectangle* box = NULL;

			bool autoResize = true;

			Amara::intVector* offset = NULL;
			Amara::intVector* center = NULL;

			Amara::LinkedList* excludedEntities = NULL;

			Camera() {
				box = new Amara::intRectangle(0, 0, 0, 0);

				scale = new Amara::floatVector(1, 1);
				anchor = new Amara::floatVector(0, 0);
				offset = new Amara::intVector(0, 0);
				center = new Amara::intVector(0, 0);

				excludedEntities = new Amara::LinkedList();

				inheritEntities = true;
			}

			Camera(int newx, int newy, int newwidth, int newheight): Camera() {
				box->x = newx;
				box->y = newy;
				box->width = newwidth;
				box->height = newheight;

				autoResize = false;
			}

			Camera(Amara::GameProperties* newProperties, int newx, int newy, int newwidth, int newheight) : Camera(newx, newy, newwidth, newheight) {
				game = newProperties;
			}

			Camera(Amara::GameProperties* newProperties, int newx, int newy, int newwidth, int newheight, Amara::floatVector* newScale) : Camera(newProperties, newx, newy, newwidth, newheight) {
				scale = newScale;
			}

			bool exclude(Amara::WorldEntity* e) {
				if (excluded(e)) {
					return false;
				}

				excludedEntities->push(e);
				return true;
			}

			bool excluded(Amara::WorldEntity* e) {
				return excludedEntities->contains(e);
			}

			void init() {
				x = 0;
				y = 0;
			}

			void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed) return;
				if (autoResize) {
					box->x = parentx;
					box->y = parenty;
					box->width = parentwidth;
					box->height = parentheight;
				}
				width = box->width;
				height = box->height;
				Amara::LinkedItem* curItem = entities->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				center->x = x + box->width/2;
				center->y = y + box->height/2;

				while (curItem != NULL) {
					Amara::WorldEntity* curEntity = (Amara::WorldEntity*) curItem->item;
					if (!excluded(curEntity)) {
						if (!curEntity->destroyed) {
							game->camera->x = floor(x) + floor((anchor->x * box->width)/scale->x);
							game->camera->y = floor(y) + floor((anchor->y * box->height)/scale->y);

							game->camScale->x = scale->x;
							game->camScale->y = scale->y;

							curEntity->draw(box->x, box->y, box->width, box->height);
							lastItem = curItem;
							curItem = curItem->next;

							game->camera->x = floor(x) + floor(offset->x * scale->x) + floor((anchor->x * box->width)/scale->x);
							game->camera->y = floor(y) + floor(offset->y * scale->y) + floor((anchor->y * box->height)/scale->y);

							game->camScale->x = scale->x;
							game->camScale->y = scale->y;
						}
						else {
							if (lastItem == NULL) {
								nextItem = curItem->next;
								curItem->destroy();
								delete curItem;
								entities->startItem = nextItem;
								curItem = nextItem;
							}
							else {
								lastItem->next = curItem->next;
								curItem->destroy();
								delete curItem;
								curItem = lastItem->next;
							}
						}
					}
					else {
						curItem = curItem->next;
					}
				}
			}

			void setOffset(int newx, int newy) {
				offset->x = newx;
				offset->y = newy;
			}

			void goTo(int newx, int newy)  {
				x = newx + floor(offset->x * scale->x);
				y = newy + floor(offset->y * scale->y);
			}

			void goTo(Amara::WorldEntity* entity) {
				follow(entity);
			}

			void glideTo(float tox, float toy, float rate) {
				x = (x - (tox + floor(offset->x * scale->x))) * rate + (tox + floor(offset->x * scale->x));
				y = (y - (toy + floor(offset->y * scale->y))) * rate + (toy + floor(offset->y * scale->y));
			}

			void mimic(Amara::Camera* other) {
				int otherWidth = other->box->width / other->scale->x;
				int otherHeight = other->box->height / other->scale->y;

				int tox = (other->x + otherWidth/2) - ((box->width/2)/scale->x) + (other->anchor->x * otherWidth) + (anchor->x * box->width);
				int toy = (other->y + otherHeight/2) - ((box->height/2)/scale->y) + (other->anchor->y * otherHeight) + (anchor->y * box->height);

				goTo(tox, toy);
			}

			void glideMimic(Amara::Camera* other, float rate) {
				int otherWidth = other->box->width / other->scale->x;
				int otherHeight = other->box->height / other->scale->y;

				float tox = (other->x + otherWidth/2) - ((box->width/2)/scale->x) + (other->anchor->x * otherWidth) + (anchor->x * box->width);
				float toy = (other->y + otherHeight/2) - ((box->height/2)/scale->y) + (other->anchor->y * otherHeight) + (anchor->y * box->height);

				glideTo(tox, toy, rate);
			}

			void follow(Amara::WorldEntity* entity) {
				if (entity->destroyed) return;
				int tx = entity->x;
				int ty = entity->y;

				int tox = tx - ((box->width/2)/scale->x);
				int toy = ty - ((box->height/2)/scale->y);
				goTo(tox, toy);
			}

			void glideFollow(Amara::WorldEntity* entity, float rate) {
				if (entity->destroyed) return;
				int tx = entity->x;
				int ty = entity->y;

				int tox = tx - ((box->width/2)/scale->x);
				int toy = ty - ((box->height/2)/scale->y);

				glideTo(tox, toy, rate);
			}

			void destroy() {
				if (destroyed) return;
				destroyed = true;
				excludedEntities->destroy();
				delete excludedEntities;
			}
	};

	/*
	 * Handles loading, creation and updating of scenes with their events.
	 * Implementation:
	 * Inherit scene and implement preload, create, update.
	 */
	class Scene: public Amara::WorldEntity {
		public:
			Amara::Camera* mainCamera = NULL;
			Amara::LinkedList* cameras = NULL;

			Amara::Loader* recLoad = NULL;

			bool autoResize = true;

			bool running = false;

			char* key;

			Scene() {
				scale = new Amara::floatVector(1, 1);
				becomeDirector();
			}

			Scene(char* newkey): Scene() {
				key = newkey;
			}

			Scene(int newx, int newy, int newwidth, int newheight): Scene() {
				x = newx;
				y = newy;
				width = newwidth;
				height = newheight;
				autoResize = false;
			}

			Scene(char* newkey, int newx, int newy, int newwidth, int newheight): Scene(newx, newy, newwidth, newheight) {
				key = newkey;
			}

			int start(bool makeLoader) {
				if (autoResize) {
					x = 0;
					y = 0;
					width = game->width;
					height = game->height;
				}

				destroyed = false;
				running = true;

				entities = new Amara::LinkedList();

				if (makeLoader) {
					load = new Amara::Loader(game);
					recLoad = load;
				}
				else {
					load  = game->load;
				}

				mainCamera = new Amara::Camera();
				mainCamera->entities = entities;
				mainCamera->load = load;
				mainCamera->game = game;
				mainCamera->init();

				cameras = new Amara::LinkedList();

				preload();
				create();
			}

			int start() {
				start(true);
			}

			virtual Amara::WorldEntity* add(Amara::WorldEntity* newEntity) {
				return Amara::WorldEntity::add(newEntity);
			}

			virtual Amara::WorldEntity* add(Amara::Camera* newCamera) {
				newCamera->game = game;
				newCamera->entities = entities;
				newCamera->load = load;
				newCamera->parent = this;
				newCamera->init();
				newCamera->create();
				newCamera->preload();

				cameras->push(newCamera);

				return newCamera;
			}

			void run() {
				if (destroyed || paused) return;
				Amara::Actor::run();
				update();
				runPhysics();
				runCameras();
				runEntities();
			}

			void runCameras() {
				mainCamera->run();

				Amara::LinkedItem* curItem = cameras->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				while (curItem != NULL) {
					Amara::Camera* curCam = (Amara::Camera*) curItem->item;
					if (!curCam->destroyed) {
						curCam->run();
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							cameras->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}

			void draw(int parentx, int parenty, int parentwidth, int parentheight) {
				if (destroyed) return;
				if (autoResize) {
					x = 0;
					y = 0;
					width = parentwidth;
					height = parentheight;
				}
				mainCamera->draw(x, y, width, height);

				Amara::LinkedItem* curItem = cameras->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;

				while (curItem != NULL) {
					Amara::Camera* curCam = (Amara::Camera*) curItem->item;
					if (!curCam->destroyed) {
						curCam->draw(x, y, width, height);
						lastItem = curItem;
						curItem = curItem->next;
					}
					else {
						if (lastItem == NULL) {
							nextItem = curItem->next;
							curItem->destroy();
							delete curItem;
							cameras->startItem = nextItem;
							curItem = nextItem;
						}
						else {
							lastItem->next = curItem->next;
							curItem->destroy();
							delete curItem;
							curItem = lastItem->next;
						}
					}
				}
			}

			void closeGame() {
				game->closeGame();
			}

			void destroyCameras() {

			}

			void destroy() {
				if (destroyed) return;
				Amara::WorldEntity::destroy();
				destroyCameras();
				destroyed = true;
				running = false;

				if (recLoad == load) {
					load->destroy();
					delete load;
				}
			}

		protected:
			SDL_Renderer* gRenderer = NULL;
			SDL_Surface* gSurface = NULL;

	};

	/*
	 * Manages the scenes of a game.
	 */
	class SceneManager {
		public:
			SceneManager(Amara::GameProperties* newProperties) {
				game = newProperties;
				scenes = new Amara::LinkedHashMap(10);
			}

			void add(char* key, Amara::Scene* newScene) {
				newScene->game = game;
				scenes->add(key, newScene);
			}

			// Begin running all the scenes available.
			void run() {
				Amara::LinkedItem* curSceneItem = scenes->getStart();
				Amara::Scene* curScene = NULL;

				while (curSceneItem != NULL) {
					curScene = (Amara::Scene*) (curSceneItem->item);

					if (curScene->running) {
						curScene->run();
					}

					curSceneItem = curSceneItem->next;
				}
			}

			void draw() {
				Amara::LinkedItem* curSceneItem = scenes->getStart();
				Amara::Scene* curScene = NULL;

				while (curSceneItem != NULL) {
					curScene = (Amara::Scene*) (curSceneItem->item);

					if (curScene->running) {
						curScene->draw(0, 0, game->resolution->width, game->resolution->height);
					}

					curSceneItem = curSceneItem->next;
				}
			}

			void start(char* key, bool makeLoader) {
				Amara::Scene* scene = (Amara::Scene*) (scenes->get(key));
				if (scene == NULL) {
					printf("Scene %s not found.\n", key);
					return;
				}
				printf("Starting Scene: %s\n", key);
				scene->start(makeLoader);
			}

			void start(char* key) {
				start(key, false);
			}

		private:
			Amara::GameProperties* game = NULL;
			Amara::LinkedHashMap* scenes = NULL;
	};

	class SceneStarter {
		public:
			SceneStarter(SceneManager* newSm) {
				sm = newSm;
			}

			void start(char* key, bool makeLoader) {
				sm->start(key, makeLoader);
			}

			void start(char* key) {
				sm->start(key);
			}

			Amara::Scene* add(char* key, Amara::Scene* newscene) {
				sm->add(key, newscene);
				return newscene;
			}

		private:
			SceneManager* sm = NULL;
	};

	/*
	 * Manages file reading and writing
	 */
	class File {
		public:
			char* path = NULL;
			SDL_RWops* file = NULL;

			bool success = false;

			char* buffer = NULL;
			int size = 0;

			File(char* newpath) {
				path = newpath;

				preload();
				create();
			}

			/*
			 * Modes:
			 * r - Open a file for reading. The file must exist.
			 * w - Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.
			 * a - Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist.

			 * r+ - Open a file for update both reading and writing. The file must exist.

			 * w+ - Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file.

			 * a+ - Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist.

			 * Put 'b' at the end to read/write to a binary file (i.e. executable) (e.g. "rb, w+b").
			 */
			bool open(char* mode) {
				file = SDL_RWFromFile(path, mode);
				size = SDL_RWsize(file);

				if (file != NULL) {
					success = true;
					return true;
				}
				else {
					success = true;
					return false;
				}
			}

			bool open() {
				return open("r+");
			}

			char* read() {
				bool closeFile = false;
				if (file == NULL) {
					closeFile = true;
					if (!open("r+")) {
						return NULL;
					}
				}

				buffer = (char*)malloc(size);
				SDL_RWread(file, buffer, size, 1);

				if (closeFile) {
					close();
				}

				return buffer;
			}

			bool write(char* item) {
				bool closeFile = false;
				if (file == NULL) {
					closeFile = true;
					if (!open("a+")) {
						return false;
					}
				}

				SDL_RWwrite(file, item, strlen(item) * sizeof(char), 1);

				if (closeFile) {
					close();
				}

				return true;
			}

			bool close() {
				if (file == NULL) return false;
				SDL_RWclose(file);
				file = NULL;

				return true;
			}

			bool print() {
				if (buffer != NULL) {
					printf("%s", buffer);
					return true;
				}
				return false;
			}

			char* std::string() {
				return buffer;
			}

			void destroy() {
				if (buffer != NULL) {
					free(buffer);
				}
				if (file != NULL) {
					close();
				}
			}

			virtual void preload() {}
			virtual void create() {}
			virtual void update() {}
	};

	/*
	 * The main class object that manages the entire game
	 */
	class Game {
		public:
			Amara::SceneManager* sceneManager = NULL;
			Amara::GameProperties* properties = NULL;
			Amara::SceneStarter* scenes = NULL;
			Amara::Loader* load = NULL;

			Amara::Keyboard* keyboard = NULL;
			Amara::Mouse* mouse = NULL;

			Amara::LinkedList* controllers = NULL;
			Amara::LinkedList* joysticks = NULL;

			int PRESSURE_MAX = 32767;

			Amara::intRectangle* display = NULL;
			Amara::intRectangle* resolution = NULL;
			Amara::intRectangle* window = NULL;

			Amara::LinkedHashMap* storage = NULL;

			Amara::WorldEntity* player = NULL;
			Amara::ControlScheme* controlScheme = NULL;

			bool dragged = false;
			bool windowFocused = true;

			bool controllerEnabled = true;

			bool lagging = false;
			int lagCounter = 0;

			Game(char* newName){
				gameName = newName;

				// Initializing properties
				properties = new Amara::GameProperties;
				properties->width = width;
				properties->height = height;
				properties->quit = quit;
				properties->vsync = vsync;
				properties->fps = fps;
				properties->tps = tps;
			}

			bool init(int startWidth, int startHeight) {
				width = startWidth;
				height = startHeight;

				// Creating the window
				if (SDL_Init(SDL_INIT_VIDEO) < 0) {
					printf("Game Error: Failed to initialize Video.");
					return false;
				}

				if (SDL_Init(SDL_INIT_AUDIO) < 0) {
					printf("Game Error: Failed to initialize Audio.");
					return false;
				}

				if (controllerEnabled && SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
					printf("Game Error: Failed to initialize Game Controller.");
					return false;
				}

				

				// Get window surface
				gSurface = SDL_GetWindowSurface(gWindow);

				// Fill the surface white
				// Background color
				SDL_FillRect(gSurface, NULL, SDL_MapRGB(gSurface->format, 0x00, 0x00, 0x00));

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);

				// Setting up the Renderer
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL) {
					printf("Game Error: Renderer failed to start. SDL Error: %s\n", SDL_GetError());
					return false;
				}

				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("Game Error: Could not load assets. SDL_image Error: %s\n", IMG_GetError());
					return false;
				}

				 //Initialize SDL_ttf
				 if(TTF_Init() == -1) {
					 printf( "Game Error: SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					 return false;
				}

				//Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
					printf("Game Error: SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
					return false;
				}

				//Initialize Joysticks and Controllers
				controllers = new Amara::LinkedList();

				sceneManager = new Amara::SceneManager(properties);
				scenes = new Amara::SceneStarter(sceneManager);

				keyboard = new Amara::Keyboard();
				mouse = new Amara::Mouse();

				storage = new Amara::LinkedHashMap(10);

				SDL_DisplayMode dm;
				if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
					printf("Game Error: Unable to detect display. Error: %s\n", SDL_GetError());
					return false;
				}
				display = new Amara::intRectangle(0, 0, dm.w, dm.h);
				resolution = new Amara::intRectangle(0, 0, width, height);
				window = new Amara::intRectangle(0, 0, width, height);
				SDL_GetWindowPosition(gWindow, &window->x, &window->y);
				printf("Game Info: Display width: %d, Display height: %d\n", dm.w, dm.h);

				// Initialize game properties
				properties->gWindow = gWindow;
				properties->gSurface = gSurface;
				properties->gRenderer = gRenderer;
				properties->sceneManager = sceneManager;
				properties->scenes = scenes;
				properties->keyboard = keyboard;
				properties->mouse = mouse;
				properties->controllers = controllers;
				properties->width = width;
				properties->height = height;
				properties->display = display;
				properties->resolution = resolution;
				properties->window = window;
				properties->storage = storage;

				load = new Amara::Loader(properties);
				properties->load = load;

				fpsTimer.start();

				setResolution(width, height);

				return true;
			}

			// For when the player closes the game
			void close() {
				// Quit Game
				quit = true;

				//Destroy window
				SDL_FreeSurface(gSurface);
				gSurface = NULL;

				SDL_DestroyRenderer(gRenderer);
				SDL_DestroyWindow(gWindow);

				//Quit SDL subsystems
				Mix_CloseAudio();
				Mix_Quit();
				TTF_Quit();
				IMG_Quit();
				SDL_Quit();
			}

			void start(char* startKey) {
				// Game loop
				sceneManager->start(startKey);

				while (!quit) {
					manageFPSStart();

					// Draw Screen
					drawScreen();

					// Manage frame catch up and slow down
					manageFPSEnd();
				}
				close();
			}

			void giveProperties() {
				// Give properties
				properties->width = width;
				properties->height = height;
				properties->quit = quit;
				properties->vsync = vsync;
				properties->fps = fps;
				properties->tps = tps;
				properties->lps = lps;
				properties->player = player;
				properties->controlScheme = controlScheme;
			}

			void reclaimProperties() {
				// Reclaim properties
				width = properties->width;
				height = properties->height;
				if (!quit) quit = properties->quit;
				vsync = properties->vsync;
				fps = properties->fps;
				tps = properties->tps;
				lps = properties->lps;
				player = properties->player;
				controlScheme = properties->controlScheme;
			}

			void handleEvents() {
				// Handle events on queue
				keyboard->manage();
				mouse->manage();

				manageControllers();

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						keyboard->press(e.key.keysym.sym);
					}
					else if (e.type == SDL_KEYUP) {
						keyboard->release(e.key.keysym.sym);
					}
					else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						mouse->x = (mx * resolution->width/properties->width);
						mouse->y = (my * resolution->height/properties->height);

						if (e.type == SDL_MOUSEBUTTONDOWN) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								mouse->leftButton->press();
							}
							else if (e.button.button == SDL_BUTTON_RIGHT) {
								mouse->rightButton->press();
							}
						}
						else if (e.type == SDL_MOUSEBUTTONUP) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								mouse->leftButton->release();
							}
							else {
								mouse->rightButton->release();
							}
						}
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_MOVED)) {
						dragged = true;
						properties->dragged = true;
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_LEAVE)) {
						windowFocused = false;
						properties->windowFocused = false;
					}
					else if (e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_ENTER)) {
						windowFocused = true;
						properties->windowFocused = true;
					}
					else if (e.type == SDL_CONTROLLERDEVICEADDED) {
						SDL_GameController* nsdlc = SDL_GameControllerOpen(e.cdevice.which);

						Amara::Controller* newcontroller = new Amara::Controller(nsdlc);
						printf("Game Info: Controller connected, Name: %s\n", SDL_GameControllerName(nsdlc));
						controllers->push(newcontroller);
					}
					else if (e.type == SDL_CONTROLLERBUTTONDOWN) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
						getController(controller)->press(e.cbutton.button);
					}
					else if (e.type == SDL_CONTROLLERBUTTONUP) {
						SDL_GameController* controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
						getController(controller)->release(e.cbutton.button);
					}
				}
			}

			void manageFPSStart() {
				capTimer.start();
			}

			void manageFPSEnd() {
				//If frame finished early
				if (quit) return;
				int totalWait = 0;
				logicDelay = 0;
				lagging = false;
				properties->lagging = false;
				if (fps < lps) {
					for (int i = 1; i < lps/fps; i++) {
						sceneManager->run();
					}
				}
				else if (fps > lps) {
					logicDelay = fps/lps;
				}

				int frameTicks = capTimer.getTicks();
				if (frameTicks < tps) {
					//Wait remaining time
					totalWait += (tps - frameTicks);
				}
				else if (frameTicks > tps) {
					if (dragged) {
						dragged = false;
						properties->dragged = false;
					}
					else {
						// Checking for lag
						if (tps < (float)frameTicks * 0.5) {
							lagging = true;
							properties->lagging = true;
							lagCounter += 1;
						}

						// Framerate catch up.
						for (int i = 0; i < (frameTicks - tps); i++) {
							if (frameCounter >= logicDelay) {
								giveProperties();
								sceneManager->run();
								handleEvents();
								frameCounter = 0;
								reclaimProperties();
								if (quit) return;
							}
							frameCounter += 1;
						}
					}
				}
				if (totalWait > 0) {
					SDL_Delay(totalWait);
				}
			}
			

			void drawScreen() {
				giveProperties();
				SDL_RenderClear(gRenderer);
				if (frameCounter >= logicDelay) {
					sceneManager->run();
					handleEvents();
					frameCounter = 0;
				}
				frameCounter += 1;

				sceneManager->draw();
				
				reclaimProperties();

				// Draw to renderer
				SDL_RenderPresent(gRenderer);
			}

			Amara::Controller* getController(SDL_GameController* givenController) {
				Amara::LinkedItem* curItem = controllers->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;
				Amara::Controller* curController = NULL;

				while (curItem != NULL) {
					curController = (Amara::Controller*) curItem->item;
					if (curController->controller == givenController) {
						return curController;
					}
					curItem = curItem->next;
				}

				return NULL;
			}

			Amara::Controller* getController(int index) {
				return properties->getController(index);
			}

			int numControllers() {
				return properties->numControllers();
			}

			void manageControllers() {
				Amara::LinkedItem* curItem = controllers->startItem;
				Amara::LinkedItem* lastItem = NULL;
				Amara::LinkedItem* nextItem = NULL;
				Amara::Controller* curController = NULL;

				while (curItem != NULL) {
					curController = (Amara::Controller*) curItem->item;
					nextItem = curItem->next;
					if (!SDL_GameControllerGetAttached(curController->controller)) {
						printf("Game Info: Controller disconnected, Name: %s\n", SDL_GameControllerName(curController->controller));
						if (lastItem == NULL) {
							controllers->startItem = nextItem;
						}
						else {
							lastItem->next = nextItem;
						}

						curController->destroy();
						delete curController;
						delete curItem;
						curItem = nextItem;

					}
					else {
						curController->ls->pushforce->x = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_LEFTX)/PRESSURE_MAX;
						curController->ls->pushforce->y = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_LEFTY)/PRESSURE_MAX;

						curController->rs->pushforce->x = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_RIGHTX)/PRESSURE_MAX;
						curController->rs->pushforce->y = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_RIGHTY)/PRESSURE_MAX;

						curController->lt->pressure = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)/PRESSURE_MAX;
						curController->rt->pressure = (float)SDL_GameControllerGetAxis(curController->controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)/PRESSURE_MAX;

						curController->manage();

						lastItem = curItem;
						curItem = nextItem;
					}
				}
			}

			void setFPS(int newFps, bool lockLogicSpeed) {
				fps = newFps;
				tps = 1000 / fps;
				if (!lockLogicSpeed) {
					lps = newFps;
				}
			}

			void setFPS(int newFps) {
				setFPS(newFps, false);
			}

			void setFPS(int newFps, int newLps) {
				fps = newFps;
				lps = newLps;
				tps = 1000 / fps;
			}

			void setLogicTickRate(int newRate) {
				lps = newRate;
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
				SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
			}

			void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b) {
				SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
			}

			void resizeWindow(int neww, int newh) {
				properties->resizeWindow(neww, newh);
			}

			void setWindowPosition(int newx, int newy) {
				properties->setWindowPosition(newx, newy);
			}

			void setResolution(int neww, int newh) {
				properties->setResolution(neww, newh);
			}

			void windowedFullScreen() {
				properties->windowedFullScreen();
			}

			Amara::File* createFile(char* path) {
				Amara::File* newfile = new Amara::File(path);
				return NULL;
			}

		protected:
			int width = 0;
			int height = 0;

			bool quit = false;

			bool vsync = false;
			int fps = 60;
			int tps = 1000 / fps;
			int lps = 60;
			LTimer fpsTimer;
			LTimer capTimer;

			int frameCounter = 0;
			int logicDelay = 0;

			SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

			char* gameName = NULL;

			SDL_Event e;
	};
}

#endif
