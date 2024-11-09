#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class BallisticKnife : public Item {
public:
    BallisticKnife() : Item("ballistic_knife", ItemType::CUSTOM) {
        maxAmmo = 1;
        currentAmmo = maxAmmo;
        reloadTime = 1.5f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();

        if (currentAmmo <= 0) {
            player.sendMessage("Votre couteau balistique est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < reloadTime) {
            player.sendMessage("Le couteau balistique recharge...");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre couteau balistique est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, 1);
            player.sendMessage("Votre couteau balistique a été rechargé avec succès.");
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
        Projectile &knife = player.spawnProjectile(ProjectileType::SNOWBALL);
        knife.setDamage(25);
        knife.setVelocity(player.getDirection() * 3.0f);
        Entity* target = findTarget(player);
        if (target) {
            knife.setTarget(target);
            player.sendMessage("Couteau balistique lancé. Cible trouvée.");
        } else {
            player.sendMessage("Couteau balistique lancé");
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
    Item::registerItem<BallisticKnife>("ballistic_knife");
}