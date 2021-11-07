#include <dirent.h>
#include <unistd.h>

#include "menu.h"
#include "mapper.h"
#include "rom.h"
#include "cpu.h"
#include "sdl_backend.h"

namespace GUI {

    using namespace std;


    Entry::Entry(string label, function<void()> callback, int x, int y) : callback(callback), x(x), y(y)
    {
        setLabel(label);
    }

    Entry::~Entry()
    {
        SDL_DestroyTexture(whiteTexture);
        SDL_DestroyTexture(redTexture);
    }

    void Entry::setLabel(string label)
    {
        this->label = label;

        if (whiteTexture != nullptr) SDL_DestroyTexture(whiteTexture);
        if (redTexture   != nullptr) SDL_DestroyTexture(redTexture);

        whiteTexture = gen_text(label, { 255, 255, 255 });
        redTexture   = gen_text(label, { 255,   0,   0 });
    }

    void Entry::render()
    {
        render_texture(selected ? redTexture : whiteTexture, getX(), getY());
    }


    ControlEntry::ControlEntry(string action, SDL_Scancode* key, int x, int y) : key(key),
        Entry::Entry(
            action,
            [&]{ keyEntry->setLabel(SDL_GetScancodeName(*(this->key) = query_key())); },
            x,
            y)
    {
        this->keyEntry = new Entry(SDL_GetScancodeName(*key), []{}, TEXT_RIGHT, y);
    }

    ControlEntry::ControlEntry(string action, int* button, int x, int y) : button(button),
        Entry::Entry(
            action,
            [&]{ keyEntry->setLabel(to_string(*(this->button) = query_button())); },
            x,
            y)
    {
        this->keyEntry = new Entry(to_string(*button), []{}, TEXT_RIGHT, y);
    }


    void Menu::add(Entry* entry)
    {
        if (entries.empty())
            entry->select();
        entry->setY(entries.size() * FONT_SZ);
        entries.push_back(entry);
    }

    void Menu::clear()
    {
        for (auto entry : entries)
            delete entry;
        entries.clear();
        cursor = 0;
    }

    void Menu::update(u8 select)
    {
        int oldCursor = cursor;

        if ((select == 32) and cursor < entries.size() - 1) {
            cursor++;
        }
        else if ((select == 16) and cursor > 0) {
            cursor--;
        }

        entries[oldCursor]->unselect();
        entries[cursor]->select();

        if (select == 1)
            entries[cursor]->trigger();
    }

    void Menu::render()
    {
        for (auto entry : entries)
            entry->render();
    }


    void FileMenu::change_dir(string dir)
    {
        /*
        clear();

        struct dirent dirp;
        DIR dp = opendir(dir.c_str());

        add(new Entry("../",
                        [=]{ change_dir("../"); },
                        0));

        while ((dirp = readdir(dp)) != NULL)
        {
            string name = dirp.d_name;
            string path = dir + "/" + name;

            if (name[0] == '.' and name != "..") continue;


            if (S_ISDIR(dirp.d_stat.st_mode))
            {
                add(new Entry(name + "/",
                            [=]{ change_dir(path); },
                            0));
            }
            else if (name.size() > 4 and name.substr(name.size() - 4) == ".nes")
            {
                add(new Entry(name,
                            [=]{ 
                                if(get_rom_status()) {
                                    end_emulation();
                                    exit_sdl_thread();
                                    GUI::stop_main_run();
                                    SDL_Delay(200);
                                    load_rom(path.c_str(), false);
                                }
                                else {
                                    load_rom(path.c_str(), false);
                                }
                                toggle_pause(); },
                            0));
            }
        }
        closedir(dp);
        */
       load_rom("app0:/mario.nes", false);
    }

    FileMenu::FileMenu()
    {
        printf("In fileMenu constructor\n");
        char cwd[512];

        // change_dir(getcwd(cwd, 512));
        change_dir("app0:");
    }


}