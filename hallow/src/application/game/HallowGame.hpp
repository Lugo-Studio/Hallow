//
// Created by galex on 5/4/2021.
//

#ifndef HALLOW_HALLOWGAME_HPP
#define HALLOW_HALLOWGAME_HPP

namespace Hallow {
  class HallowGame {
  public:
    HallowGame() = default;
    ~HallowGame() = default;

    // TODO: Move to lifetime events class
    void onPreStart();
    void onStart();
    void onPostStart();

    void onPreUpdate();
    void onUpdate();
    void onPostUpdate();

    void onPreEnd();
    void onEnd();
    void onPostEnd();

    HallowGame(const HallowGame&) = delete;
    HallowGame& operator=(const HallowGame&) = delete;
  private:

  };
}


#endif //HALLOW_HALLOWGAME_HPP
