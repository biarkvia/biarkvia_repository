<?php

namespace App\Controller\Api;

use App\Entity\Role;
use App\Operation\EmployeeOperation;
use App\Operation\RoleOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

#[Route('/api/employees/{id}/roles', name: 'api_employees_roles_')]
class RoleController extends AbstractController
{
    public function __construct(
        private EmployeeOperation $employeeOperation,
        private RoleOperation $roleOperation
    ) {}

    #[Route('', name: 'add', methods: ['POST'])]
    public function addRole(int $id, Request $request, RoleRepository $roleRepository): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $data = json_decode($request->getContent(), true);
        $roleId = $data['roleId'] ?? null;
        $roleTitle = $data['title'] ?? null;

        if (!$roleId && !$roleTitle) {
            return $this->json(['error' => 'Role ID or title is required'], Response::HTTP_BAD_REQUEST);
        }

        $role = $roleId ? $roleRepository->find($roleId) : null;

        if (!$role && $roleTitle) {
            $role = new Role();
            $role->setTitle($roleTitle);
            $role->setIsVisible(true);
            $this->roleOperation->store($role);
        }

        $employee->addRole($role);
        $this->employeeOperation->store($employee);

        return $this->json(['message' => 'Role added successfully'], Response::HTTP_CREATED);
    }

    #[Route('/{roleId}', name: 'remove', methods: ['DELETE'])]
    public function removeRole(int $id, int $roleId): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $role = $this->roleOperation->findById($roleId);

        if (!$role) {
            return $this->json(['error' => 'Role not found'], Response::HTTP_NOT_FOUND);
        }

        $employee->removeRole($role);
        $this->employeeOperation->store($employee);

        return $this->json(['message' => 'Role deleted'], Response::HTTP_OK);
    }
}
