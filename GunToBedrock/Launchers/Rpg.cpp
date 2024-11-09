#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class RPG7 : public Item {
public:
    RPG7() : Item("rpg7", ItemType::CUSTOM) {
        maxAmmo = 1;
        currentAmmo = maxAmmo;
        reloadTime = 5.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        
        if (currentAmmo <= 0) {
            player.sendMessage("Votre RPG-7 est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < reloadTime) {
            player.sendMessage("Le RPG-7 recharge...");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre RPG-7 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::EXPLOSIVE)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::EXPLOSIVE, 1);
            player.sendMessage("Votre RPG-7 a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas de roquettes pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float reloadTime;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &rocket = player.spawnProjectile(ProjectileType::FIREBALL);
        rocket.setDamage(100);
        rocket.setVelocity(player.getDirection() * 12.0f);
        Entity* target = findTarget(player);
        if (target) {
            rocket.setTarget(target);
            player.sendMessage("Roquette lancée ! Cible trouvée.");
        } else {
            player.sendMessage("Aucune cible détectée. Roquette lancée aléatoirement.");
        }

        player.playSound(SoundType::LAUNCH);
    }

    Entity* findTarget(Player &player) {
        for (Entity &entity : Minecraft::getEntities()) {
            if (entity.isEnemy(player)) {
                return &entity;
            }
        }
        return nullptr;
    }
};

extern "C" void init() {
    Item::registerItem<RPG7>("rpg7");
}