#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class Crossbow : public Item {
public:
    Crossbow() : Item("crossbow", ItemType::CUSTOM) {
        maxAmmo = 1;
        currentAmmo = maxAmmo;
        reloadTime = 2.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        
        if (currentAmmo <= 0) {
            player.sendMessage("Votre arbalète est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < reloadTime) {
            player.sendMessage("L'arbalète recharge...");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre arbalète est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, 1);
            player.sendMessage("Votre arbalète a été rechargée avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas de flèches pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float reloadTime;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &arrow = player.spawnProjectile(ProjectileType::ARROW);
        arrow.setDamage(10);
        arrow.setVelocity(player.getDirection() * 3.0f);
        Entity* target = findTarget(player);
        if (target) {
            arrow.setTarget(target);
            player.sendMessage("Flèche lancée. Cible trouvée.");
        } else {
            player.sendMessage("Aucune cible détectée. Flèche tirée.");
        }

        player.playSound(SoundType::SHOOT);
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
    Item::registerItem<Crossbow>("crossbow");
}