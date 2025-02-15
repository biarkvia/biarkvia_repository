<?php

namespace App\Controller\Api;

use App\Operation\RoleOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\Routing\Annotation\Route;

#[Route('/api/roles', name: 'api_roles_')]
class RoleTitleController extends AbstractController
{
    public function __construct(private readonly RoleOperation $roleOperation)
    {
    }

    #[Route('/titles', name: 'titles', methods: ['GET'])]
    public function getRoleTitles(): JsonResponse
    {
        $roles = $this->roleOperation->findAll();
        $roleTitles = array_map(fn($role) => $role->getTitle(), $roles);

        return $this->json($roleTitles);
    }
}
