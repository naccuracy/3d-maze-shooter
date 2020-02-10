//  ☀️
#include "game.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

Game::Game() {
  falling = 0;
  fallingTime = 0;
  fallingEnd = 10;
  fallingVelocity = 10;
  inited = 0;
  debug_ind = 0;

  Keys_picked = 0;
  Score = 0;
  Bul_count = 50;
  Live_count = 3;
  time = 0;
  maxtime = 2000;
  day_time_flag = 0;
  LN = 0;
  LM = 0;
  LABIRINT = {};
  gameover = false;
  bot_flag = 1;
  bot_count = 0;
  med_count = 0;
  amm_count = 0;
  bblk_count = 0;
  ublk_count = 0;
  sbblk_count = 0;
  keys_count = 0;
  doors_count = 0;
  cam_flag = 0;

  mat_specular[0] = 1.0;
  mat_specular[1] = 1.0;
  mat_specular[2] = 1.0;
  mat_specular[3] = 1.0;
  mat_shininess[0] = 10.0;
  white_light[0] = 1.0;
  white_light[1] = 1.0;
  white_light[2] = 1.0;
  white_light[3] = 1.0;
  grey_light[0] = 0.0;
  grey_light[1] = 0.0;
  grey_light[2] = 0.0;
  grey_light[3] = 0.0;

  mainloopActive = false;
  win_flag = 0;

  SoundFlag = false;
  SoundDevice = NULL;
  SoundContext = NULL;
}

Game::~Game() {
}

ALboolean Game::ALFWLoadWaveToBuffer(const char *szWaveFile,
                                     ALuint uiBufferID) {

  std::uint8_t channels;
  std::int32_t sampleRate;
  std::uint8_t bitsPerSample;
  std::int32_t soundDataSize = 0;
  char *soundData =
      load_wav(szWaveFile, channels, sampleRate, bitsPerSample, soundDataSize);
  if (!soundData) {
    std::cerr << "ERROR: Could not load wav" << std::endl;
    return AL_FALSE;
  }
  ALenum format;
  if (channels == 1 && bitsPerSample == 8)
    format = AL_FORMAT_MONO8;
  else if (channels == 1 && bitsPerSample == 16)
    format = AL_FORMAT_MONO16;
  else if (channels == 2 && bitsPerSample == 8)
    format = AL_FORMAT_STEREO8;
  else if (channels == 2 && bitsPerSample == 16)
    format = AL_FORMAT_STEREO16;
  else {
    std::cerr << "ERROR: unrecognised wave format: " << channels
              << " channels, " << bitsPerSample << " bps" << std::endl;
    return AL_FALSE;
  }
  alGetError();
  ALboolean bReturn = AL_FALSE;
  alBufferData(uiBufferID, format, soundData, soundDataSize, sampleRate);
  if (alGetError() == AL_NO_ERROR) {
    bReturn = AL_TRUE;
  }
  delete[] soundData;
  return bReturn;
}

void Game::Gameover() {
  Live_count = 0;
  intr->f2->edit2->caption = "You Lose...";
  intr->f2->edit2->Show();
  gameover = true;
  alSourcePlay(SoundSources[gameover_sound]);
}

void Game::CameraMovement() {
  float st = 2.0;
  float nar = 0.0;
  int flag;
  int i, j, n;
  Point3f temp_pos;
  if (falling) {
    fallingTime++;
    cam0.GoV(-fallingVelocity);
    if (fallingTime >= fallingEnd) {
      Gameover();
      falling = 0;
      fallingTime = 0;
    }
    return;
  }

  if (keyboard[SDLK_f]) {
    falling = 1;
  }
  if (keyboard[SDLK_w]
   || keyboard[SDLK_s]
   || keyboard[SDLK_a]
   || keyboard[SDLK_d]) {
    if (keyboard[SDLK_w]
     || keyboard[SDLK_d]) {
      nar = 1.0;
    }
    if (keyboard[SDLK_s]
     || keyboard[SDLK_a]) {
      nar = -1.0;
    }
    flag = 0;
    if (keyboard[SDLK_w] || keyboard[SDLK_s]) {
      cam0.GoN(st * nar);
      temp_pos = cam0.Position() + cam0.N() * (2.0 * nar);
    }
    if (keyboard[SDLK_a] || keyboard[SDLK_d]) {
      cam0.GoU(st * nar);
      temp_pos = cam0.Position() + cam0.U() * (2.0 * nar);
    }
    n = deny.size();
    for (j = 0; j < n; j += 2) {
      if (isInside(temp_pos, deny[j + 1], deny[j])) {
        flag = 1;
        break;
      }
    }
    n = yama.size();
    for (j = 0; j < n; j += 2) {
      if (isInside(temp_pos, yama[j + 1], yama[j])) {
        falling = 1;
        return;
      }
    }
    j = isPosInOneOf(temp_pos, doors, sm1.min, sm1.max);
    if (j != -1) {
      flag = 1;
    }
    j = isPosInOneOf(temp_pos, bad_blocks, sm1.min, sm1.max);
    if (j != -1) {
      flag = 1;
    }
    j = isPosInOneOf(temp_pos, up_blocks, sm1.min, sm1.max);
    if (j != -1) {
      flag = 1;
    }
    j = isPosInOneOf(temp_pos, bots, sm.min, sm.max);
    if (j != -1) { 
      bots[j].actor.RotateV(180.0);
      bots[j].actor.GoN(bots[j].velocity);
      flag = 1;
      alSourcePlay(SoundSources[kick_sound]);
    }
    j = isPosInOneOf(temp_pos, medpaks, hrt.min, hrt.max);
    if (j != -1) {
      medpaks[j].alive = 0;
      alSourcePlay(SoundSources[pick_med]);
      Live_count++;
      intr->f2->updateLivesCount(Live_count);
    }
    j = isPosInOneOf(temp_pos, ammunitions, ammo.min, ammo.max);
    if (j != -1) {
      ammunitions[j].alive = 0;
      alSourcePlay(SoundSources[pick_ammo]);
      Bul_count += 10;
      intr->f2->updateBuletsCount(Bul_count);
    }
    j = isPosInOneOf(temp_pos, keys, key.min, key.max);
    if (j != -1) {
      keys[j].alive = 0;
      alSourcePlay(SoundSources[pick_ammo]);
      Keys_picked += 1;
      intr->f2->updateKeysCount(Keys_picked);
    }
    if (flag) {
      if (keyboard[SDLK_w] || keyboard[SDLK_s]) {
        cam0.GoN((-1.0) * st * nar);
      }
      if (keyboard[SDLK_a] || keyboard[SDLK_d]) {
        cam0.GoU((-1.0) * st * nar);
      }
    }
  }

  if (keyboard[SDLK_q]) {
    cam0.GoV(st);
  }
  if (keyboard[SDLK_e]) {
    cam0.GoV(-st);
    if (cam0.Position().data[1] < 10.0)
      cam0.GoV(st);
  }
  cam1.set(cam0.Position() + Point3f(0, 100, 0), cam0.Position(),
           Point3f(0, 0, 1));
  n = win.size();
  j = 0;
  if (!win_flag)
    for (i = 0; i < n; i += 2) {
      if (isInside(cam0.Position(), win[i + 1], win[i])) {
        sprintf(debug_str, "You win!!!!");
        win_flag = 1;
        alSourcePlay(SoundSources[win_sound]);
        intr->f2->edit2->caption.assign(debug_str);
        intr->f2->edit2->Show();
        gameover = true;
        break;
      }
    }
  sprintf(debug_str, "min: %f;%f;%f max %f;%f;%f", sm1.min.data[0],
          sm1.min.data[1], sm1.min.data[2], sm1.max.data[0], sm1.max.data[1],
          sm1.max.data[2]);
  intr->f2->edit1->caption.assign(debug_str);
  return;
}

void Game::deinit() {
  deinitSound();
  clearPointers();
  if (intr) {
    delete intr;
  }
  font1.Clear();
  deny.clear();
  win.clear();
  return;
}

int Game::deinitSound() {
  for (int i = 0; i < sounds; ++i) {
    ALint state = AL_STOPPED;
    alGetSourcei(SoundSources[i], AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED) {
      alSourceStop(SoundSources[i]);
    }
  }
  alDeleteSources(sounds, SoundSources);
  alDeleteBuffers(sounds, SoundBuffers);
  SoundContext = alcGetCurrentContext();
  SoundDevice = alcGetContextsDevice(SoundContext);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(SoundContext);
  alcCloseDevice(SoundDevice);
  return 1;
}

void Game::drawLabirint(int mode) {
  int i, j;
  float delta = 10.0;
  GLfloat mtrx[16];

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glColor3f(1.0, 1.0, 1.0);

  if (mode) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture[wall_tex]);
    glPushMatrix();
    glTranslatef(-5.0, delta + 5.0, 5.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.1, 0.0);
    glTexCoord2f(0.0, 100.0);
    glVertex3f(0.0, 0.1, -LM * delta);
    glTexCoord2f(100.0, 100.0);
    glVertex3f(LN * delta, 0.1, -LM * delta);
    glTexCoord2f(100.0, 0.0);
    glVertex3f(LN * delta, 0.1, 0.0);
    glEnd();
    glPopMatrix();
  } else {
    glDisable(GL_TEXTURE_2D);
  }

  float idelta, jdelta;
  glBindTexture(GL_TEXTURE_2D, myTexture[wall_tex]);
  for (i = 0; i < LN; i++) {
    idelta = i * delta;
    for (j = 0; j < LM; j++) {
      jdelta = -j * delta;
      glPushMatrix();
      glTranslatef(idelta, 0, jdelta);
      switch (LABIRINT[i][j]) {
      case 1: 
        glTranslatef(0, delta, 0);
        glColor3f(1.0, 1.0, 1.0);
        break;
      case 9:
        glTranslatef(0, delta, 0);
        glColor3f(0.2, 0.2, 0.9);
        break;
      case 15:
        glTranslatef(0, delta, 0);
        glColor3f(0.2, 0.2, 0.2);
        break;
      case 14:
        glTranslatef(0, -delta, 0);
        glColor3f(0.0, 0.0, 0.0);
        break;
      case 17:
        glTranslatef(0, -delta, 0);
        glColor3f(0.0, 0.5, 0.0);
        break;
      case 24:
      case 25:
        glTranslatef(0, delta, 0);
        glColor3f(0.9, 0.5, 0.9);
        break;
      default:
        glColor3f(0.0, 0.5, 0.0);
      }
      sm1.actor.GetActorMatrix(mtrx);
      glMultMatrixf(mtrx);
      sm1.DrawFlatTexturedReferenece();
      glPopMatrix();
/*
10 - бафф (8 патронов)
11 - рычаг
13 - супер-пупер-блок(стена с ключом)
16 - бафф (160 патронов)
A(17) - место под установку блоков
B(18) - ключ (в каждой комнате есть ключ, без которого нельзя выйти из
лабиринта) С(19) - дверь, которая открывается после прохождения испытания в
данной комнате D(20) - сундук с подсказкой E(21) - клетки, на которые нужно
наступать, чтобы пройти испытание Q(22) - клетки, которые падают, если на них
наступить W(23) - лопата (расчищалка завалов)
*/
    }
  }
  if (mode) {
    glDisable(GL_TEXTURE_2D);
  }
  return;
}

void Game::IdleFunc() {
  int i, j = 0, n, flag;
  string str;
  if (intr->newGame) {
    str = os.Value("home") + os.Value("map");
    LoadLabirint((char *)str.c_str());
    intr->newGame = 0;
    gameover = 0;
    intr->f2->edit2->Hide();
    intr->f2->updateBuletsCount(Bul_count);
    intr->f2->updateLivesCount(Live_count);
    intr->f2->updateDeadsCount(Score);
  }
  n = deny.size();
  Point3f bulet_pos;
  intr->OnIdle();
  if (gameover) {
    return;
  }
  CameraMovement();
  if (bulet.alive) {
    bulet.actor.GoN(bulet.velocity);
    bulet_pos = bulet.actor.Position() + bulet.actor.N() * bulet.velocity;
    for (j = 0; j < n; j += 2) {
      if (isInside(bulet_pos, deny[j + 1], deny[j])) {
        bulet.alive = false;
        break;
      }
    }
    j = isPosInOneOf(bulet_pos, bots, sm.min, sm.max);
    if (j != -1) {
      bulet.alive = false;
      bots[j].alive = false;
      alSourcePlay(SoundSources[smert_sound]);
      Score++;
      intr->f2->updateDeadsCount(Score);
    }
    j = isPosInOneOf(bulet_pos, bad_blocks, sm1.min, sm1.max);
    if (j != -1) {
      bulet.alive = false;
      bad_blocks[j].alive--;
    }
    j = isPosInOneOf(bulet_pos, doors, sm1.min, sm1.max);
    if (j != -1) {
      bulet.alive = false;
    }
    j = isPosInOneOf(bulet_pos, up_blocks, sm1.min, sm1.max);
    if (j != -1) {
      bulet.alive = false;
    }
    bulet.time += 1.0;
    if (bulet.time >=
        bulet.rotate_time) {
      bulet.time = 0;
      bulet.alive = false;
    }
  }
  nebo.actor.RotateV(0.05);

  if (keyboard[SDLK_c]) {
    bot_flag = 1;
  }
  if (keyboard[SDLK_v]) {
    bot_flag = 0;
  }

  for (i = 0; i < med_count; i++) {
    if (medpaks[i].alive) {
      medpaks[i].actor.RotateV(medpaks[i].velocity);
    }
  }

  for (i = 0; i < amm_count; i++) {
    if (ammunitions[i].alive) {
      ammunitions[i].actor.RotateV(ammunitions[i].velocity);
    }
  }

  for (i = 0; i < keys_count; i++) {
    if (keys[i].alive) {
      keys[i].actor.RotateV(keys[i].velocity);
    }
  }

  for (i = 0; i < ublk_count; i++) {
    up_blocks[i].actor.GoV(up_blocks[i].velocity * up_blocks[i].rotate_flag);
    bulet_pos = up_blocks[i].actor.Position();
    bulet_pos.data[1] += sm1.min.data[1];
    flag = 0;
    if (isInside(bulet_pos, cam0.Position() + sm.min,
                 cam0.Position() + sm.max)) {
      flag = 1;
      alSourcePlay(SoundSources[lose_live_sound]);
      Live_count--;
      intr->f2->updateLivesCount(Live_count);
    }
    if (!flag) {
      j = isPosInOneOf(bulet_pos, bots, sm.min, sm.max);
      if (j != -1) {
        flag = 1;
        bots[j].alive = false;
        alSourcePlay(SoundSources[smert_sound]);
      }
    }
    if (flag) {
      up_blocks[i].rotate_flag = -1.0 * up_blocks[i].rotate_flag;
      up_blocks[i].time = up_blocks[i].rotate_time - up_blocks[i].time;
      up_blocks[i].actor.GoV(up_blocks[i].velocity * up_blocks[i].rotate_flag);
      flag = 0;
    }
    up_blocks[i].time += 1.0;
    if (up_blocks[i].time >=
        up_blocks[i].rotate_time) {
      up_blocks[i].time = 0.0;
      up_blocks[i].rotate_flag = -1.0 * up_blocks[i].rotate_flag;
    }
  }
  if (bot_flag) {
    for (i = 0; i < bot_count; i++) {
      if (bots[i].alive) {
        bots[i].actor.GoN(bots[i].velocity);
        flag = 0;
        bulet_pos = bots[i].actor.Position() + bots[i].actor.N() * 2.0;
        if (isPosInOneOf(bulet_pos, up_blocks, sm.min, sm.max) != -1) {
          flag = 1;
        }
        if (isPosInOneOf(bulet_pos, doors, sm.min, sm.max) != -1) {
          flag = 1;
        }
        if (isPosInOneOf(bulet_pos, bad_blocks, sm.min, sm.max) != -1) {
          flag = 1;
        }
        for (j = 0; j < bot_count; j++) {
          if (bots[j].alive && i != j) 
            if (isInside(bulet_pos, bots[j].actor.Position() + sm.min,
                         bots[j].actor.Position() + sm.max)) {
              flag = 1;
              break;
            }
        }
        for (j = 0; j < n; j += 2) {
          if (isInside(bulet_pos, deny[j + 1], deny[j])) {
            flag = 1;
            break;
          }
        }
        if (isInside(bulet_pos, cam0.Position() + sm.min,
                     cam0.Position() + sm.max)) {
          flag = 1;
          alSourcePlay(SoundSources[lose_live_sound]);
          Live_count--;
          intr->f2->updateLivesCount(Live_count);
        }
        if (flag) {
          bots[i].actor.GoN(-bots[i].velocity);
          bots[i].actor.RotateV(90.0 *
                                bots[i].rotate_flag);
          flag = 0;
        }
        if (bots[i].time >=
            bots[i].rotate_time) 
        {
          bots[i].time = 0.0;
          bots[i].actor.RotateV(180.0);
          if (bots[i].rotate_flag == -1)
            bots[i].rotate_flag = 1;
          else
            bots[i].rotate_flag = -1;
        }
        bots[i].time += 1.0;
      }
    }
  }
  if (time == maxtime) {
    time = 0;
    day_time_flag = !day_time_flag;
  }

  time++;
  if (Live_count <= 0) {
    Gameover();
  }

  return;
}

void Game::init() {
  string str;
  int i;
  for (i = 0; i < 1000; i++) {
    keyboard[i] = 0;
  }
  intr->setLoadingProgress(0, "Loading meshes");
  str = os.Value("home") + os.Value("mesh");
  sm1.LoadFromCSV((char *)str.c_str());
  sm1.Normalize(sm_flat);
  sm1.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  sm1.CalcBoundingBox();

  str = os.Value("home") + os.Value("heart");
  hrt.LoadFromCSV((char *)str.c_str());
  hrt.Normalize(sm_flat);
  hrt.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  hrt.CalcBoundingBox();
  str = os.Value("home") + os.Value("ammo");
  ammo.LoadFromCSV((char *)str.c_str());
  ammo.Normalize(sm_flat);
  ammo.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  ammo.CalcBoundingBox();
  str = os.Value("home") + os.Value("key");
  key.LoadFromCSV((char *)str.c_str());
  key.Normalize(sm_flat);
  key.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  key.CalcBoundingBox();
  sm.LoadSphere(4, 10, 6);
  sm.Normalize(sm_smooth);
  sm.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  prepareHeroTexCoord(&sm, 10, 6);
  sm.CalcBoundingBox();
  cam0pos0 = Point3f(15, 15, 15);
  str = os.Value("home") + os.Value("map");
  LoadLabirint((char *)str.c_str());
  int R = LN;
  if (R < LM)
    R = LM;
  nebo.LoadSphere((R + 1) * 10, 36, 36);
  nebo.SwapFaces();
  nebo.Normalize(sm_smooth);
  nebo.actor.set(Point3f(LN * 5, 0, -LM * 5), Point3f(LM * 5, 0, -LN * 5 + 1),
                 Point3f(0, 1, 0));
  prepareSkyTexCoord(&nebo, 36, 36);
  bulet_mesh.LoadSphere(2, 6, 6);
  bulet_mesh.Normalize(sm_flat);
  bulet_mesh.CalcBoundingBox();
  cam0.set(cam0pos0, Point3f(LN * 5, 10, LM * 5), Point3f(0, 1, 0));
  cam1.set(cam0pos0 + Point3f(0, 200, 0), cam0pos0, Point3f(0, 0, 1));
  bird.set(Point3f(LN * 5.0, R * 10.0, -LM * 5.0),
           Point3f(LN * 5.0, 0.0, -LM * 5.0), Point3f(0, 0, 1));
  intr->setLoadingProgress(30, "Loading textures");
  initTextures();
  intr->setLoadingProgress(90, "Loading sounds");
  initSound();
  mainloopActive = true;
  intr->f2->buletsImg->SetImage(myTexture[bulet_tex]);
  intr->f2->updateBuletsCount(Bul_count);
  intr->f2->livesImg->SetImage(myTexture[live_tex]);
  intr->f2->updateLivesCount(Live_count);
  intr->f2->deadsImg->SetImage(myTexture[dead_tex]);
  intr->f2->updateDeadsCount(Score);
  intr->f2->keysImg->SetImage(myTexture[key_pic]);
  intr->f2->updateKeysCount(Keys_picked);

  intr->f2->map_tex_id = myTexture[map_tex];
  intr->f3->ver = (char *)glGetString(GL_VERSION);
  intr->f3->vendor = (char *)glGetString(GL_VENDOR);
  intr->f3->renderer = (char *)glGetString(GL_RENDERER);
  intr->f3->initInfo();
  intr->setLoadingProgress(100, "Loading complete");
  inited = 1;
  return;
}

int Game::initBots() {
  int i;
  for (i = 0; i < bot_count; i++) {
    bot b;
    b.alive = 1;
    b.velocity = 2;
    b.actor.set(Point3f((LN / 2) * 10 - i * 20, 15, 15),
                Point3f((LN / 2) * 10 - i * 20, 15, 10), Point3f(0, 40, 0));
    b.time = 0.0;
    b.rotate_flag = rand() % 2;
    b.rotate_time = rand() % 300 + 100;
    bots.emplace_back(b);
  }
  bulet.alive = 0;
  bulet.velocity = 7.0;
  bulet.actor.set(Point3f(0, 0, 0), Point3f(0, 0, 1), Point3f(0, 1, 0));
  bulet.time = 0.0;
  bulet.rotate_flag = 0;
  bulet.rotate_time = 50.0;

  for (i = 0; i < med_count; i++) {
    bot medpak;
    medpak.alive = 1;
    medpak.velocity = 1;
    medpak.actor.set(Point3f((LN / 2) * 10 - i * 20, 15, 15),
                     Point3f((LN / 2) * 10 - i * 20, 15, 10),
                     Point3f(0, 40, 0));
    medpak.time = 0.0;
    medpak.rotate_flag = 0;
    medpak.rotate_time = 300.0;
    medpaks.emplace_back(medpak);
  }
  for (i = 0; i < amm_count; i++) {
    bot ammunition;
    ammunition.alive = 1;
    ammunition.velocity = 1.5;
    ammunition.actor.set(Point3f((LN / 2) * 10 - i * 20, 15, 15),
                         Point3f((LN / 2) * 10 - i * 20, 15, 10),
                         Point3f(0, 40, 0));
    ammunition.time = 0.0;
    ammunition.rotate_flag = 0;
    ammunition.rotate_time = 300.0;
    ammunitions.emplace_back(ammunition);
  }
  for (i = 0; i < bblk_count; i++) {
    bot bb;
    bb.alive = 2;
    bb.velocity = 0;
    bb.time = 0.0;
    bb.rotate_flag = 0;
    bb.rotate_time = 0;
    bad_blocks.emplace_back(bb);
  }
  for (i = bblk_count; i < bblk_count + sbblk_count; i++) {
    bot bb;
    bb.alive = 10;
    bb.velocity = 0;
    bb.time = 0.0;
    bb.rotate_flag = 0;
    bb.rotate_time = 0;
    bad_blocks.emplace_back(bb);
  }
  for (i = 0; i < ublk_count; i++) {
    bot ub;
    ub.alive = 1;
    ub.velocity = 0.5;
    ub.time = 0.0;
    ub.rotate_flag = 1;
    ub.rotate_time = 25.0;
    up_blocks.emplace_back(ub);
  }
  for (i = 0; i < keys_count; i++) {
    bot key;
    key.alive = 1;
    key.velocity = 1.5;
    key.time = 0.0;
    key.rotate_flag = 0.0;
    key.rotate_time = 300.0;
    keys.emplace_back(key);
  }
  for (i = 0; i < doors_count; i++) {
    bot door;
    door.alive = 1;
    door.velocity = 0.5;
    door.time = 0.0;
    door.rotate_flag = 1;
    door.rotate_time = 25.0;
    doors.emplace_back(door);
  }
  return 1;
}

void Game::initOptions() {
  std::string str;
  if (os.openFile((char *)"settings.csv")) {
    std::cerr << "options load error" << std::endl;
    return;
  }
  str = os.Value("home") + os.Value("font");
  font1.Clear();
  font1.LoadFont((char *)str.c_str());
  font1.SetSize(40);
  if (intr)
    delete intr;
  intr = new MyInterface(atoi(os.Value("width").c_str()),
                         atoi(os.Value("height").c_str()), &font1);
  intr->font_size = 40;
  intr->fnt_clr.Set(1.0, 1.0, 0.7, 1.0);
  intr->Init(&os);
  return;
}

int Game::initTextures() {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(tex_count, myTexture);
  loadTexture("ava_tex", ava_tex, 0);
  loadTexture("wall_tex", wall_tex, 0);
  loadTexture("flour_tex", flour_tex, 0);
  loadTexture("day_tex", day_tex, 0);
  loadTexture("night_tex", night_tex, 0);
  loadTexture("hunter_tex", hunter_tex, 0);
  loadTexture("live_pic", live_tex, 1);
  loadTexture("dead_pic", dead_tex, 1);
  loadTexture("bulet_pic", bulet_tex, 1);
  loadTexture("key_pic", key_pic, 1);
  initmaptex();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  return 1;
}

int Game::initSound() {
  string str;
  SoundDevice = alcOpenDevice(NULL);
  if (SoundDevice) {
    SoundContext = alcCreateContext(SoundDevice, NULL);
    alcMakeContextCurrent(SoundContext);
  } else {
    std::cerr << "Sound Device open failed!" << std::endl;
    return 0;
  }
  alGetError();
  alGenBuffers(sounds, SoundBuffers);
  if (alGetError() != AL_NO_ERROR) {
    std::cerr << "Something bad when sound buffer created!" << std::endl;
    return 0;
  }
  str = os.Value("home") + os.Value("kick_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[kick_sound])) {
    std::cerr << "Something bad when sound kick_sound loaded" << std::endl;
  }
  str = os.Value("home") + os.Value("theme_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[theme_sound])) {
    std::cerr << "Something bad when sound theme_sound loaded" << std::endl;
  }
  str = os.Value("home") + os.Value("gameover_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[gameover_sound])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("lose_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[lose_live_sound])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("live_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[pick_med])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("ammo_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[pick_ammo])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("smert_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[smert_sound])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("bah_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[bah_sound])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  str = os.Value("home") + os.Value("win_sound");
  if (!ALFWLoadWaveToBuffer(str.c_str(), SoundBuffers[win_sound])) {
    std::cerr << "Something bad when sound file loaded" << __LINE__
              << std::endl;
  }
  alGenSources(sounds, SoundSources);
  if (alGetError() != AL_NO_ERROR) {
    std::cerr << "Something bad when sound sources created!" << __LINE__
              << std::endl;
    return 0;
  }
  ALfloat listenerPos[] = {0.0, 0.0, 0.0};
  ALfloat listenerVel[] = {0.0, 0.0, 0.0};
  ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
  alListenerfv(AL_POSITION, listenerPos);
  if (alGetError() != AL_NO_ERROR) {
    std::cerr << "alListenerfv POSITION" << __LINE__ << std::endl;
    return 0;
  }
  alListenerfv(AL_VELOCITY, listenerVel);
  if (alGetError() != AL_NO_ERROR) {
    std::cerr << "alListenerfv VELOCITY : " << __LINE__ << std::endl;
    return 0;
  }
  alListenerfv(AL_ORIENTATION, listenerOri);
  if (alGetError() != AL_NO_ERROR) {
    std::cerr << "alListenerfv ORIENTATION : " << __LINE__ << std::endl;
    return 0;
  }
  for (int i = 0; i < sounds; i++) {
    alSourcei(SoundSources[i], AL_BUFFER, SoundBuffers[i]);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "cannot attach sound buffer to sound source" << __LINE__
                << std::endl;
    }
    alGetError();
    alSourcef(SoundSources[i], AL_PITCH, 1.0f);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "alSourcef 0 AL_PITCH : " << __LINE__ << std::endl;
    }
    alGetError();
    alSourcef(SoundSources[i], AL_GAIN, 1.0f);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "alSourcef 0 AL_GAIN : " << __LINE__ << std::endl;
    }
    alGetError();
    alSourcefv(SoundSources[i], AL_POSITION, listenerPos);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "alSourcef 0 AL_POSITION : " << __LINE__ << std::endl;
    }
    alGetError();
    alSourcefv(SoundSources[i], AL_VELOCITY, listenerVel);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "alSourcef 0 AL_VELOCITY : " << __LINE__ << std::endl;
    }
    alGetError();
    alSourcei(SoundSources[i], AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
      std::cerr << "alSourcef 0 AL_LOOPING true : " << __LINE__ << std::endl;
    }
  }
  return 1;
}

int Game::isInside(Point3f p, Point3f A, Point3f B) {
  if (p[0] < A[0] || p[1] < A[1] || p[2] < A[2])
    return 0;
  if (p[0] > B[0] || p[1] > B[1] || p[2] > B[2])
    return 0;
  return 1;
}

int Game::LoadLabirint(char *fl) {
  float delta = 10.0;
  Score = 0;
  Bul_count = 50;
  Live_count = 3;
  win_flag = 0;
  FILE *f = NULL;
  int i, j;
  int val;
  f = fopen(fl, "rt");
  if (f != NULL) {
    clearPointers();

    fscanf(f, "%d;%d", &LN, &LM);
    LABIRINT.resize(LN, vector<int>(LM, -1));
    fscanf(f, "%d;%d;%d;%d;%d;%d;%d;%d", &bot_count, &med_count, &amm_count,
           &bblk_count, &ublk_count, &sbblk_count, &keys_count, &doors_count);
    bots.reserve(bot_count);
    medpaks.reserve(med_count);
    ammunitions.reserve(amm_count);
    bad_blocks.reserve(bblk_count + sbblk_count);
    up_blocks.reserve(ublk_count);
    keys.reserve(keys_count);
    doors.reserve(doors_count);
    initBots();
    for (i = 0; i < LN; i++) {
      for (j = 0; j < LM; j++) {
        fscanf(f, "%d;", &val);
        LABIRINT[i][j] = val;
      }
      fgetc(f);
    }
    fclose(f);
  } else {
    printf("labirint :: error");
  }

  deny.clear();
  win.clear();
  yama.clear();
  int flag = 0, flwin = 0;
  int flyama = 0;
  Point3f p0, p1;
  int k = 0;
  int m = 0;
  int a = 0;
  int bb = 0;
  int ub = 0;
  int ky = 0;
  int dr = 0;
  float idelta, jdelta;
  for (i = 0; i < LN; i++) {
    idelta = i * delta;
    flag = 0;
    flwin = 0;
    flyama = 0;
    for (j = 0; j < LM; j++) {
      jdelta = -j * delta;
      if (LABIRINT[i][j] == 2) {
        cam0pos0 = Point3f(idelta, delta, jdelta);
        cam0.set(cam0pos0, Point3f(LN * 5, delta, -LM * 5),
                 Point3f(0, 1, 0));
        cam1.set(cam0pos0 + Point3f(0, 200, 0), cam0pos0,
                 Point3f(0, 0, 1));
        continue;
      }
      if (LABIRINT[i][j] == 4) {
        if (k < bot_count) {
          bots[k].actor.set(Point3f(idelta, delta, jdelta),
                            Point3f(idelta, delta, jdelta + 5),
                            Point3f(0, 40, 0));
          k++;
        }
      }
      if (LABIRINT[i][j] == 18) {
        if (ky < keys_count) {
          keys[ky].actor.set(Point3f(idelta, delta, jdelta),
                             Point3f(idelta, delta, jdelta + 5),
                             Point3f(0, 40, 0));
          ky++;
        }
      }
      if (LABIRINT[i][j] == 19) {
        if (dr < doors_count) {
          doors[dr].actor.set(Point3f(idelta, delta, jdelta),
                              Point3f(idelta, delta, jdelta + 5),
                              Point3f(0, 40, 0));
          dr++;
        }
      }
      if (LABIRINT[i][j] == 5) {
        if (m < med_count) {
          medpaks[m].actor.set(Point3f(idelta, delta + 2.0, jdelta),
                               Point3f(idelta, delta + 2.0, jdelta + 1),
                               Point3f(0, 40, 0));
          m++;
        }
      }
      if (LABIRINT[i][j] == 6) {
        if (a < amm_count) {
          ammunitions[a].actor.set(Point3f(idelta, delta + 2.0, jdelta),
                                   Point3f(idelta, delta + 2.0, jdelta + 1),
                                   Point3f(0, 40, 0));
          a++;
        }
      }
      if (LABIRINT[i][j] == 7) {
        if (ub < ublk_count) {
          up_blocks[ub].actor.set(Point3f(idelta, delta, jdelta),
                                  Point3f(idelta, delta, jdelta + 1),
                                  Point3f(0, 40, 0));
          ub++;
        }
      }
      if (LABIRINT[i][j] == 8) {
        if (bb < bblk_count) {
          bad_blocks[bb].actor.set(Point3f(idelta, delta, jdelta),
                                   Point3f(idelta, delta, jdelta + 1),
                                   Point3f(0, 40, 0));
          bb++;
        }
      }
      if (LABIRINT[i][j] == 1) {
        if (!flag) {
          flag = 1;
          p0 = Point3f(idelta, delta, jdelta) + sm1.max;
          deny.push_back(p0);
        }
        if (j == LM - 1) {
          flag = 0;
          p1 = Point3f(idelta, delta, jdelta) + sm1.min;
          deny.push_back(p1);
        }
      } else {
        if (flag) {
          flag = 0;
          p1 = Point3f(idelta, delta, 0.0 - (j - 1) * delta) + sm1.min;
          deny.push_back(p1);
        }
      }
      if (LABIRINT[i][j] == 3) {
        if (!flwin) {
          flwin = 1;
          p0 = Point3f(idelta, delta, jdelta) + sm1.max;
          win.push_back(p0);
        }
        if (j == LM - 1) {
          flwin = 0;
          p1 = Point3f(idelta, delta, jdelta) + sm1.min;
          win.push_back(p1);
        }
      } else {
        if (flwin) {
          flwin = 0;
          p1 = Point3f(idelta, delta, 0.0 - (j - 1) * delta) + sm1.min;
          win.push_back(p1);
        }
      }
      if (LABIRINT[i][j] == 22 || LABIRINT[i][j] == 14) {
        if (!flyama) {
          flyama = 1;
          p0 = Point3f(idelta, delta, jdelta) + sm1.max;
          yama.push_back(p0);
        }
        if (j == LM - 1) {
          flyama = 0;
          p1 = Point3f(idelta, delta, jdelta) + sm1.min;
          yama.push_back(p1);
        }
      } else {
        if (flyama) {
          flyama = 0;
          p1 = Point3f(idelta, delta, 0.0 - (j - 1) * delta) + sm1.min;
          yama.push_back(p1);
        }
      }
    }
  }
  for (i = 0; i < LN; i++) {
    idelta = i * delta;
    for (j = 0; j < LM; j++) {
      jdelta = -j * delta;
      if (LABIRINT[i][j] == 12) {
        if (bb < bblk_count + sbblk_count) {
          bad_blocks[bb].actor.set(Point3f(idelta, delta, jdelta),
                                   Point3f(idelta, delta, jdelta + 1),
                                   Point3f(0, 40, 0));
          bb++;
        }
      }
    }
  }
  return 0;
}

int Game::loadTexture(string opt_name, int tex_id, int rgba) {
  nBitmap24 bitmap;
  string str = os.Value("home") + os.Value(opt_name);
  bitmap.load((char *)str.c_str());
  glBindTexture(GL_TEXTURE_2D, myTexture[tex_id]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  if (rgba) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.Width(), bitmap.Height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 bitmap.getRGBADataWithTransparentColor(0, 255, 255));
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.Width(), bitmap.Height(), 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bitmap.getRGBData());
  }
  return 1;
}

int Game::prepareHeroTexCoord(SimpleMesh *mesh, int seg, int slice) {
  int i, j, l = 0, k = 0, n, m;
  Point2f p, d;
  mesh->texcoords.clear();
  mesh->tex_faces.clear();
  d = Point2f(0, 0);
  p = d;
  vector<int> dots;
  for (i = 1; i < slice; i++) {
    l = 0;
    if (i == slice / 2 - 1) {
      l = 1;
      p.data[1] = 0.0;
    }
    if (i == slice / 2) {
      l = 1;
      p.data[1] = 0.5;
    }
    if (i == slice / 2 + 1) {
      l = 1;
      p.data[1] = 1.0;
    }
    for (j = 0; j < seg; j++) {
      k = 0;
      if (l) {
        if (j == 0) {
          k = 1;
          p.data[0] = 0.5;
        }
        if (j == 1) {
          k = 1;
          p.data[0] = 1.0;
        }
        if (j == seg - 1) {
          k = 1;
          p.data[0] = 0.0;
        }
      }
      if (k) {
        dots.push_back(mesh->texcoords.size());
        mesh->texcoords.push_back(p);
      } else
        mesh->texcoords.push_back(d);
    }
  }
  p = Point2f(0, 0);
  mesh->texcoords.push_back(d);
  mesh->texcoords.push_back(d);
  mesh->tex_faces.assign(mesh->faces.begin(), mesh->faces.end());
  k = mesh->tex_faces.size();
  l = dots.size();
  m = mesh->points.size() - 1;
  for (i = 0; i < k; i++) {
    n = 0;
    for (j = 0; j < l; j++) {
      if (mesh->tex_faces[i].hasVertex(dots[j]))
        n++;
      if (n == 3)
        break;
    }
    if (n != 3) {
      mesh->tex_faces[i] = Face3i(m, m, m);
    }
  }
  return 0;
}

int Game::prepareSkyTexCoord(SimpleMesh *mesh, int seg, int slice) {
  int i, j;
  Point2f p, d;
  mesh->texcoords.clear();
  mesh->tex_faces.clear();
  d = Point2f(0, 0);
  float du = 1.0 / (float)slice, dv = 1.0 / (float)seg;
  vector<int> d0, dn;
  for (i = 1; i < slice; i++) {
    d.data[1] = du * i;
    for (j = 0; j < seg; j++) {
      d.data[0] = dv * j;
      if (j == 0)
        d0.push_back(mesh->texcoords.size());
      if (j == seg - 1)
        dn.push_back(mesh->texcoords.size());
      mesh->texcoords.push_back(d);
    }
  }
  p = Point2f(1, 0.5);
  mesh->texcoords.push_back(p);
  p = Point2f(0.0, 0.5);
  mesh->texcoords.push_back(p);
  mesh->tex_faces.assign(mesh->faces.begin(), mesh->faces.end());

  int k, k0, kn, n = mesh->tex_faces.size();
  int m0 = d0.size(), mn = dn.size();
  for (i = 0; i < n; i++) {
    k0 = 0;
    for (k = 0; k < m0; k++) {
      if (mesh->tex_faces[i].hasVertex(d0[k]))
        k0++;
    }
    kn = 0;
    for (k = 0; k < mn; k++) {
      if (mesh->tex_faces[i].hasVertex(dn[k]))
        kn++;
    }
    if (k0 == 1 && kn == 2) {
      for (k = 0; k < m0; k++) {
        if (mesh->tex_faces[i].a == d0[k])
          mesh->tex_faces[i].a = mesh->tex_faces[i].b;
        if (mesh->tex_faces[i].b == d0[k])
          mesh->tex_faces[i].b = mesh->tex_faces[i].a;
        if (mesh->tex_faces[i].c == d0[k])
          mesh->tex_faces[i].c = mesh->tex_faces[i].b;
      }
    }
    if (k0 == 2 && kn == 1) {
      for (k = 0; k < mn; k++) {
        if (mesh->tex_faces[i].a == dn[k])
          mesh->tex_faces[i].a = mesh->tex_faces[i].b;
        if (mesh->tex_faces[i].b == dn[k])
          mesh->tex_faces[i].b = mesh->tex_faces[i].a;
        if (mesh->tex_faces[i].c == dn[k])
          mesh->tex_faces[i].c = mesh->tex_faces[i].b;
      }
    }
  }
  d0.clear();
  dn.clear();
  return 0;
}

void Game::RedrawInterface() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, intr->width, 0, intr->height, 10.0, -10.0);
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  intr->RedrawItem();
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  return;
}

void Game::RedrawScene() {
  int i;
  Point3f heartc = Point3f(1.0, 0.0, 0.0);
  Point3f ammoc = Point3f(0.8, 0.8, 0.5);
  GLfloat cview[16];
  int mts_div_2 = map_tex_size / 2.0;
  for (int Q = 0; Q < 2; Q++) {
    if (Q) {
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      if (!keyboard[SDLK_b]) {
        cam0.GetCameraMatrix(cview);
      } else {
        bird.GetCameraMatrix(cview);
      }
    }
    if (!Q) {
      glViewport(0, 0, map_tex_size, map_tex_size);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(-mts_div_2, mts_div_2, -mts_div_2, mts_div_2, 0, 2000);
      cam1.GetCameraMatrix(cview);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLoadMatrixf(cview);
    light_position = cam0.Position() + Point3f(0, 25, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position.data);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    if (day_time_flag) {
      glBindTexture(GL_TEXTURE_2D, myTexture[day_tex]);
    } else {
      glBindTexture(GL_TEXTURE_2D, myTexture[night_tex]);
    }
    glColor3f(1.0, 1.0, 1.0);
    nebo.DrawWithTexCoords(sm_smooth);
    glDisable(GL_TEXTURE_2D);
    if (!day_time_flag) {
      glLightfv(GL_LIGHT0, GL_AMBIENT, grey_light);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, grey_light);
      glLightfv(GL_LIGHT0, GL_SPECULAR, grey_light);
    }
    if (Q) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, myTexture[wall_tex]);
      glColor3f(1.0, 0.0, 0.0);
      for (i = 0; i < bblk_count + sbblk_count; i++) {
        if (i == bblk_count) {
          glColor3f(0.6, 0.0, 0.0);
        }
        if (bad_blocks[i].alive > 0) {
          glPushMatrix();
          bad_blocks[i].actor.GetActorMatrix(cview);
          glMultMatrixf(cview);
          sm1.DrawFlatTexturedReferenece();
          glPopMatrix();
        }
      }
      glColor3f(0.1, 0.1, 0.0);
      for (i = 0; i < ublk_count; i++) {
        glPushMatrix();
        up_blocks[i].actor.GetActorMatrix(cview);
        glMultMatrixf(cview);
        sm1.DrawFlatTexturedReferenece();
        glPopMatrix();
      }
      glColor3f(0.1, 0.5, 0.5);
      for (i = 0; i < doors_count; i++) {
        glPushMatrix();
        doors[i].actor.GetActorMatrix(cview);
        glMultMatrixf(cview);
        sm1.DrawFlatTexturedReferenece();
        glPopMatrix();
      }
      glBindTexture(GL_TEXTURE_2D, myTexture[ava_tex]);
      glColor3f(1.0, 1.0, 1.0);
      for (i = 0; i < bot_count; i++) {
        if (bots[i].alive) {
          glPushMatrix();
          bots[i].actor.GetActorMatrix(cview);
          glMultMatrixf(cview);
          sm.DrawSmoothTexturedReferenece();
          glPopMatrix();
        }
      }
      glDisable(GL_TEXTURE_2D);
      for (i = 0; i < med_count; i++) {
        if (medpaks[i].alive) {
          glPushMatrix();
          medpaks[i].actor.GetActorMatrix(cview);
          glMultMatrixf(cview);
          hrt.DrawSolidReference(heartc, sm_flat);
          glPopMatrix();
        }
      }
      for (i = 0; i < amm_count; i++) {
        if (ammunitions[i].alive) {
          glPushMatrix();
          ammunitions[i].actor.GetActorMatrix(cview);
          glMultMatrixf(cview);
          ammo.DrawSolidReference(ammoc, sm_flat);
          glPopMatrix();
        }
      }
      for (i = 0; i < keys_count; i++) {
        if (keys[i].alive) {
          glPushMatrix();
          keys[i].actor.GetActorMatrix(cview);
          glMultMatrixf(cview);
          key.DrawSolidReference(Point3f(1.0, 1.0, 0.5), sm_flat);
          glPopMatrix();
        }
      }
      if (bulet.alive) {
        glPushMatrix();
        bulet.actor.GetActorMatrix(cview);
        glMultMatrixf(cview);
        bulet_mesh.DrawSolidReference(Point3f(0.7, 0.7, 0.8), sm_flat);
        glPopMatrix();
      }
    } 
    drawLabirint(Q);
    if (!Q) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, myTexture[hunter_tex]);
      glColor3f(1.0, 1.0, 1.0);
      sm.actor = cam0;
      sm.DrawWithTexCoords(sm_smooth);
      glBindTexture(GL_TEXTURE_2D, myTexture[map_tex]);
      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, map_tex_size,
                       map_tex_size, 0);
      glDisable(GL_TEXTURE_2D);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, intr->width, intr->height);
    }
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    if (!Q) {
      glFinish();
    } else {
      glFlush();
    }
  }
  return;
}

void Game::initmaptex() {
  int n = map_tex_size * map_tex_size * 3, i;
  map_tex_ptr = new GLbyte[n];
  for (i = 0; i < n; i++) {
    map_tex_ptr[i] = 0;
  }
  glBindTexture(GL_TEXTURE_2D, myTexture[map_tex]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map_tex_size, map_tex_size, 0, GL_RGB,
               GL_UNSIGNED_BYTE, map_tex_ptr);
  delete map_tex_ptr;
}

int Game::isPosInOneOf(Point3f pos, const vector<bot> &ar, Point3f bbmin,
                       Point3f bbmax) {
  int flag = -1;
  for (size_t i = 0; i < ar.size(); ++i) {
    if (ar[i].alive > 0) {
      if (isInside(pos, ar[i].actor.Position() + bbmin,
                   ar[i].actor.Position() + bbmax)) {
        flag = i;
        break;
      }
    }
  }
  return flag;
}

int Game::OnChar(int ch) {
  intr->OnType(ch);
  return 1;
}

int Game::OnKeyDown(int key) {
  intr->OnKeyDown(key);
  if (keyboard[key] == 0) {
    keyboard[key] = 1;
  }
  return 1;
}

int Game::OnKeyUp(int key) {
  intr->OnKeyUp(key);
  if (keyboard[key] == 1) {
    keyboard[key] = 0;
  }
  return 1;
}

int Game::OnLButtonDown(short x,
                        short y) {
  intr->OnMouseDown(x, y, 1);
  if (intr->childs[intr->child_focus] == intr->mainMenu) {
    if (intr->quitFlag) {
      mainloopActive = false;
    }
  }
  if (gameover) {
    return 1;
  }
  if (bulet.alive == 0 && (!intr->gamePause)) {
    if (Bul_count > 0) {
      bulet.actor = cam0;
      bulet.alive = 1;
      alSourcePlay(SoundSources[bah_sound]);
      Bul_count--;
      intr->f2->updateBuletsCount(Bul_count);
    }
  }
  return 1;
}

int Game::OnLButtonUp(short x, short y) {
  intr->OnMouseUp(x, y, 1);
  return 1;
}

int Game::OnMouseMove(short x, short y) {
  float ang = 4.0;
  int dx;
  if (!intr->gamePause && !gameover) {
    dx = x - intr->width / 2;
    if (dx < 0) {
      cam0.RotateV(ang);
    }
    if (dx > 0) {
      cam0.RotateV(-ang);
    }
  }
  intr->OnMouseMove(x, y);
  return 1;
}

int Game::OnPaint() {
  if (!intr->gamePause) {
    RedrawScene();
  }
  RedrawInterface();
  return 1;
}

int Game::OnSize(int wi, int he) {
  intr->OnSize(wi, he);
  return 1;
}

int Game::OnTimer() {
  if (!intr->gamePause) {
    IdleFunc();
  }
  return 1;
}

void Game::clearPointers() {
  if (LABIRINT.size()) {
    for (int i = 0; i < LN; i++) {
      LABIRINT[i].clear();
    }
    LABIRINT.clear();
  }

  bots.clear();
  medpaks.clear();
  ammunitions.clear();
  bad_blocks.clear();
  up_blocks.clear();
  keys.clear();
  doors.clear();
}
