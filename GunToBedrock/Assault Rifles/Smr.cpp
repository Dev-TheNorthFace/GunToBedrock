#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class SMR : public Item {
public:
    SMR() : Item(smr, ItemType::CUSTOM) {
        maxAmmo = 20;
        currentAmmo = maxAmmo;
        fireRate = 1.5f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre SMR est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre SMR est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::BULLET, maxAmmo);
            player.sendMessage("Votre SMR a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(25);
        bullet.setVelocity(player.getDirection() * 3.0f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage(Bang. La SMR a tiré.);
    }
};

extern "C" void init() {
    Item::registerItem<SMR>(smr);
}
